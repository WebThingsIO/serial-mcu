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
#include "Packet.h"
#include "StaticArray.h"

template <size_t JSON_BUFFER_SIZE>
class Manager {
public:
  Manager(CommPort &port,
          Buffer &rxPacketBuffer, Buffer &txPacketBuffer,
          const Adapter *adapters, size_t numAdapters)
    : mPort(port),
      mRxPacket(rxPacketBuffer),
      mTxPacket(txPacketBuffer),
      mAdapters(adapters, numAdapters) {

    mRxPacket.ShowBytes(false);
    mTxPacket.ShowBytes(false);
  }

  void OnPacket(JsonObject &jsonPacket) {
    #if 1
    printf("Got a JSON packet\n");
    char printBuf[512];
    jsonPacket.prettyPrintTo(printBuf, sizeof(printBuf));
    printf("%s\n", printBuf);
    #endif

    const char *cmd = jsonPacket["messageType"];
    if (cmd == NULL) {
      printf("No messageType\n");
      return;
    }
    JsonObject &data = jsonPacket["data"];
    if (!data.success()) {
      printf("No data\n");
      return;
    }
    if (strcmp(cmd, "getAdapterCount") == 0) {
      SendAdapterCount();
    } else if (strcmp(cmd, "getAdapter") == 0) {
      size_t adapterIdx = jsonPacket["data"]["adapterIdx"];
      SendAdapter(adapterIdx);
    } else {
      printf("Unrecognized command: %s\n", cmd);
      return;
    }

    printf("Writing it back\n");
    mTxPacket.Write(mPort, printBuf, strlen(printBuf));
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
    //printf("Got a packet: '%s'\n", mRxPacket.Bytes());

    // The call to clear will cause dangling pointers for objects
    // pointing into the buffer, but they should have all been
    // completed when the last packet was processed.
    mJsonBuffer.clear();
    JsonObject &jsonPacket = mJsonBuffer.parseObject(mRxPacket.Bytes());
    if (!jsonPacket.success()) {
      printf("Got a bad JSON packet\n");
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
    mTxPacket.FillHeaderAndTrailer(len);

    mPort.Write(mTxPacket.Bytes(), mTxPacket.Length());
  }

  void SendAdapterCount() {
    JsonObject &data = mJsonBuffer.createObject();
    data["adapterCount"] = mAdapters.size();
    SendMessage("adapterCount", data);
  }

  void SendAdapter(size_t adapterIdx) {

  }

private:
  CommPort &mPort;
  Packet mRxPacket;
  Packet mTxPacket;
  StaticArray<Adapter> mAdapters;
  StaticJsonBuffer<JSON_BUFFER_SIZE> mJsonBuffer;
};

#endif // Manager_h
