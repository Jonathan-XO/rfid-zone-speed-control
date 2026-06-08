import asyncio
import socket
import websockets
import re

UDP_IP = "0.0.0.0"
UDP_PORT = 5005
WS_PORT = 8765

clients = set()

async def udp_listener():
    """Listens for UDP data and broadcasts to WebSocket clients."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))
    sock.setblocking(True)  # We'll call it in executor
    print(f"✅ Listening for UDP on {UDP_IP}:{UDP_PORT}")

    loop = asyncio.get_event_loop()

    while True:
        try:
            # ✅ Run blocking recvfrom in a thread pool (cross-platform)
            data, addr = await loop.run_in_executor(None, sock.recvfrom, 1024)
            message = data.decode("utf-8").strip()

            # Parse data from ESP8266
            zone = re.search(r"Zone:\s*([A-Za-z]+)", message)
            tag = re.search(r"Tag:\s*([A-F0-9]+)", message)
            gear = re.search(r"Gear:\s*(\d+)", message)
            speed = re.search(r"Speed:\s*([\d.]+)", message)

            zone = zone.group(1) if zone else "Unknown"
            tag = tag.group(1) if tag else "None"
            gear = gear.group(1) if gear else "0"
            speed = speed.group(1) if speed else "0.0"

            formatted = f"Speed:{speed}, Gear:{gear}, Zone:{zone}, Tag:{tag}"
            print(f"[{addr[0]}] {formatted}")

            # Broadcast to connected WebSocket clients
            if clients:
                await asyncio.gather(*(ws.send(formatted) for ws in clients))

        except Exception as e:
            print("⚠️ UDP error:", e)
            await asyncio.sleep(0.05)

async def ws_handler(websocket, path):
    clients.add(websocket)
    print("💻 Web UI connected")
    try:
        await websocket.wait_closed()
    finally:
        clients.remove(websocket)
        print("💻 Web UI disconnected")

async def main():
    ws_server = await websockets.serve(ws_handler, "0.0.0.0", WS_PORT)
    print(f"🌐 WebSocket bridge running on ws://localhost:{WS_PORT}")
    await udp_listener()

if __name__ == "__main__":
    asyncio.run(main())
