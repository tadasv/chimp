import unittest
import subprocess
from time import sleep
import uuid

import pychimp
import regression_settings


class DSNEWCommandTestCase(unittest.TestCase):
    def setUp(self):
        self.proc = subprocess.Popen(regression_settings.chimp_location)
        sleep(1)
        self.c = pychimp.ChimpClient(regression_settings.chimp_host)

    def tearDown(self):
        self.c.close()
        self.proc.kill()

    def test_dsnew(self):
        resp = self.c.dsnew()
        self.assertEqual(400, resp.response_code)
        self.assertTrue(resp.error)

        dataset_name = str(uuid.uuid4())
        resp = self.c.dsnew(dataset_name, 4)
        self.assertEqual(200, resp.response_code)
        self.assertEqual(None, resp.data)
        self.assertEqual(None, resp.error)

        resp = self.c.dsnew(dataset_name, 4)
        self.assertEqual(400, resp.response_code)
        self.assertTrue(resp.error)
