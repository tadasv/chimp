import unittest
import subprocess
from time import sleep

import pychimp
import regression_settings


class PingCommandTestCase(unittest.TestCase):
    def setUp(self):
        self.proc = subprocess.Popen(regression_settings.chimp_location)
        sleep(1)
        self.c = pychimp.ChimpClient(regression_settings.chimp_host)

    def tearDown(self):
        self.c.close()
        self.proc.kill()

    def test_ping(self):
        resp = self.c.ping()
        self.assertEqual(200, resp.response_code)
        self.assertEqual(None, resp.data)
        self.assertEqual(None, resp.error)
