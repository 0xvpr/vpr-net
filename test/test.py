#!/usr/bin/env python3

import http.client
import asyncio

from pathlib import Path

async def run(binary_path: Path) -> None:
    await asyncio.create_subprocess_exec(
        binary_path
    )

if __name__ == "__main__":
    binary_path = Path(__file__).relative_to(Path.cwd()).parent / "../bin/Release/vpr-net.exe"

    asyncio.run(run(binary_path))
    conn = http.client.HTTPConnection("127.0.0.1", 47001)

    headers   = { 'Content-type' : 'text/html' }
    body      = ""
    endpoints = [
        "/test"
    ]
    
    for endpoint in endpoints:
        conn.request("GET", "/test", body, headers)
        
        resp = conn.getresponse()
        print(resp.status, resp.reason)
        print(resp.read().decode())

    conn.close()