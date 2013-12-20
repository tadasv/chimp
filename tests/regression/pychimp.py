import msgpack
import socket

class ChimpResponse(object):
    response_code = None
    data = None
    error = None


class ChimpClient(object):
    def __init__(self, host):
        self._conn = socket.create_connection(host)
        self._command = None

    def close(self):
        self._conn.close()

    def __getattr__(self, name):
        self._command = name.upper()
        return self

    def __call__(self, *args):
        data = [self._command] + list(args)
        self._conn.sendall(msgpack.packb(data))
        resp = self._conn.recv(64 * 1024)
        resp = msgpack.loads(resp)
        obj = ChimpResponse()
        obj.response_code = resp[0]
        obj.data = resp[1]
        obj.error = resp[2]
        return obj
