#!/usr/bin/env python3

import subprocess
import http.client

import time

import asyncio


async def run():
    p = await asyncio.create_subprocess_shell(
        'bin/vpr-net-test.exe'
    )

    return p

if __name__ == "__main__":
    run()

    time.sleep(1);

    conn = http.client.HTTPConnection("127.0.0.1", 47001)

    headers = { 'Content-type' : 'text/html' }
    body = ""

    conn.request("GET", "/test", body, headers)
    
    resp = conn.getresponse()
    print(resp.status, resp.reason)
    print(resp.read().decode())

    conn.close()
