/*
 * Manager.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !defined(Manager_h)
#define Manager_h

#include <ArduinoJson.h>

#include "Adapter.h"
#include "Buffer.h"
#include "CommPort.h"
#include "Log.h"
#include "Packet.h"
#include "StaticArray.h"

class ManagerBase {
public:
  ManagerBase(Adapter &adapter)
    : mAdapter(adapter) {

    // Save back references to the Thing and ManagerBase
    mAdapter.SetManager(this);
  }
  virtual void OnPropertyChanged(const Property &property, const JsonVariant &val) = 0;

protected:
  Adapter &mAdapter;
};

template <size_t JSON_BUFFER_SIZE>
class Manager : public ManagerBase {
public:
  Manager(CommPort &port,
          Buffer &rxPacketBuffer, Buffer &txPacketBuffer,
          Adapter &adapter)
    : ManagerBase(adapter),
      mPort(port),
      mRxPacket(rxPacketBuffer),
      mTxPacket(txPacketBuffer) {

    mRxPacket.ShowBytes(false);
    mTxPacket.ShowBytes(false);
  }

  void OnPacket(JsonObject &jsonPacket) {
    #if 0
    LOG("Got a JSON packet\n");
    char printBuf[512];
    jsonPacket.prettyPrintTo(printBuf, sizeof(printBuf));
    LOG("%s\n", printBuf);
    #endif

    const char *cmd = jsonPacket["messageType"];
    if (cmd == NULL) {
      SendError("No messageType");
      return;
    }
    JsonObject &data = jsonPacket["data"];
    if (!data.success()) {
      SendError("No data");
      return;
    }
    if (strcmp(cmd, "getAdapter") == 0) {
      SendAdapter();
    } else if (strcmp(cmd, "getThingByIdx") == 0) {
      SendThing(data["thingIdx"]);
    } else if (strcmp(cmd, "getPropertyByIdx") == 0) {
      SendProperty(data["thingIdx"], data["propertyIdx"]);
    } else if (strcmp(cmd, "setProperty") == 0) {
      SetProperty(data["id"], data["name"], data["value"]);
    } else {
      SendError("Unrecognized command");
    }
  }

  virtual void OnPropertyChanged(const Property &property, const JsonVariant &val) {
    LOG("OnPropertyChanged: name = %s\n", property.Name());
    JsonObject &data = mJsonBuffer.createObject();
    const Thing *thing = property.GetThing();
    if (thing) {
      data["id"] = thing->Id();
      data["name"] = property.Name();
      data["value"] = val;
      SendMessage("propertyChanged", data);
    } else {
      SendError("propertyChanged unable to determine thing");
    }
  }

  // The basic design is that the host will only ever send commands which
  // get a single packet in response, and that the host won't send any
  // additional commands while waiting for the response.
  //
  // We need to allow for this device to send async packets to the host
  // so that the host doesn't have to poll, but the host can deal with
  // that type of buffering. This means that while we're sending, there
  // may also be incoming data.
  //
  // We can get away with a single Rx and single Tx packet buffers by
  // making sure that there is no outgoing TxPacket data before processing
  // an Rx Packet It's ok to continue to accumulate Rx chars, we just defer
  // processing until the tx packet is gone.

  bool Process() {
    int byte = mPort.ReadByte();
    if (byte < 0) {
      if (mPort.Eof()) {
        // Client disconnected
        return false;
      }
      return true;
    }
    if (!mRxPacket.ProcessByte(byte)) {
      // Packet not complete yet
      return true;
    }
    // We received a packet.
    //LOG("Got a packet: '%s'\n", mRxPacket.Bytes());

    // The call to clear will cause dangling pointers for objects
    // pointing into the buffer, but they should have all been
    // completed when the last packet was processed.
    mJsonBuffer.clear();
    JsonObject &jsonPacket = mJsonBuffer.parseObject(mRxPacket.Bytes());
    if (!jsonPacket.success()) {
      LOG("Got a bad JSON packet\n");
      return true;
    }

    OnPacket(jsonPacket);
    return true;
  }

  void SendMessage(const char *messageType, JsonObject &data) {
    JsonObject &msg = mJsonBuffer.createObject();
    msg["messageType"] = messageType;
    msg["data"] = data;

    //XXXX need to add code to drain buffer if it isn't empty

    size_t len = msg.printTo((char *)mTxPacket.WriteBytes(), mTxPacket.WriteSize());
    //LOG("Sending '%s'\n", mTxPacket.WriteBytes());
    mTxPacket.FillHeaderAndTrailer(len);

    mPort.Write(mTxPacket.Bytes(), mTxPacket.Length());
  }

  void SendAdapter() {
    SendMessage("adapter", mAdapter.JsonDescription(mJsonBuffer));
  }

  void SendError(const char *msg) {
    JsonObject &data = mJsonBuffer.createObject();
    data["msg"] = msg;
    SendMessage("error", data);
  }

  void SendThing(size_t thingIdx) {
    const Thing *thing = mAdapter.GetThing(thingIdx);
    if (!thing) {
      SendError("thing not found");
      return;
    }
    SendMessage("thing", thing->JsonDescription(mJsonBuffer));
  }

  void SendProperty(size_t thingIdx, size_t propertyIdx) {
    const Thing *thing = mAdapter.GetThing(thingIdx);
    if (!thing) {
      SendError("thing not found");
      return;
    }
    const Property *property = thing->GetProperty(propertyIdx);
    if (!property) {
      SendError("property not found");
      return;
    }
    SendMessage("property", property->JsonDescription(mJsonBuffer));
  }

  void SetProperty(const char *id,
                   const char *name,
                   const JsonVariant &value) {
    if (!id) {
      SendError("No id provided");
      return;
    }
    if (!name) {
      SendError("No name provided");
      return;
    }
    if (!value.success()) {
      SendError("No value provided");
      return;
    }
    const Thing *thing = mAdapter.GetThing(id);
    if (!thing) {
      SendError("thing not found");
      return;
    }
    const Property *property = thing->GetProperty(name);
    if (!property) {
      SendError("property not found");
      return;
    }
    property->Set(value);
  }

private:
  CommPort &mPort;
  Packet mRxPacket;
  Packet mTxPacket;
  StaticJsonBuffer<JSON_BUFFER_SIZE> mJsonBuffer;
};

#endif // Manager_h
