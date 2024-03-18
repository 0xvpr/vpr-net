#!/usr/bin/env python

import http.client
import asyncio

from pathlib import Path
from typing import NamedTuple
from asyncio.subprocess import Process

class ExpectedResult(NamedTuple):
    status: int
    reason: str

async def run(binary_path: Path) -> Process:
    return await asyncio.create_subprocess_exec(
        binary_path
    )

def test_endpoint( endpoint: str,
                   expected_result: ExpectedResult) -> bool:
    headers   = { 'Content-type' : 'text/html' }
    body      = ""
    
    conn = http.client.HTTPConnection("127.0.0.1", 47001)
    conn.request("GET", endpoint, body, headers)
    
    resp = conn.getresponse()
    print(resp.status, resp.reason)

    conn.close()
    return resp.status == expected_result.status

if __name__ == "__main__":
    binary_path = Path(__file__).relative_to(Path.cwd()).parent \
                  / "../bin/Release/vpr-net.exe"
    asyncio.run(run(binary_path))

    print( test_endpoint("/test",       expected_result=ExpectedResult(404, "Error: Not found")) )
    print( test_endpoint("/a",          expected_result=ExpectedResult(200, "")) )
    print( test_endpoint("/b",          expected_result=ExpectedResult(200, "")) )
    print( test_endpoint("/c",          expected_result=ExpectedResult(400, "")) )
    print( test_endpoint("/c?value=1",  expected_result=ExpectedResult(200, "")) )
    print( test_endpoint("/d",          expected_result=ExpectedResult(404, "")) )
    print( test_endpoint("/d?value=1",  expected_result=ExpectedResult(404, "")) )
