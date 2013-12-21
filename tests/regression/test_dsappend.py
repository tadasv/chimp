import unittest
import subprocess
from time import sleep
import uuid

import pychimp
import regression_settings


class DSAPPENDCommandTestCase(unittest.TestCase):
    def setUp(self):
        self.proc = subprocess.Popen(regression_settings.chimp_location)
        sleep(1)
        self.c = pychimp.ChimpClient(regression_settings.chimp_host)

    def tearDown(self):
        self.c.close()
        self.proc.kill()

    def test_dsappend(self):
        dataset_name = str(uuid.uuid4())
        resp = self.c.dsappend(dataset_name, [1, 2])
        self.assertTrue(resp.error)
        self.assertEqual(400, resp.response_code)

        self.c.dsnew(dataset_name, 2)
        resp = self.c.dsappend(dataset_name, [1])
        self.assertTrue(resp.error)
        self.assertEqual(400, resp.response_code)

        resp = self.c.dsappend(dataset_name, ["str"])
        self.assertTrue(resp.error)
        self.assertEqual(400, resp.response_code)

        resp = self.c.dsappend(dataset_name, [4, "str", 4, 4])
        self.assertTrue(resp.error)
        self.assertEqual(400, resp.response_code)

        resp = self.c.dslist()
        self.assertEqual(0, resp.data[0]['dimensions']['rows'])
        self.assertEqual(2, resp.data[0]['dimensions']['columns'])

        resp = self.c.dsappend(dataset_name, [4] * 2)
        self.assertFalse(resp.error)
        self.assertEqual(200, resp.response_code)

        resp = self.c.dslist()
        self.assertEqual(1, resp.data[0]['dimensions']['rows'])
        self.assertEqual(2, resp.data[0]['dimensions']['columns'])

        resp = self.c.dsappend(dataset_name, [4] * 3)
        self.assertTrue(resp.error)
        self.assertEqual(400, resp.response_code)
        resp = self.c.dslist()
        self.assertEqual(1, resp.data[0]['dimensions']['rows'])
        self.assertEqual(2, resp.data[0]['dimensions']['columns'])

        resp = self.c.dsappend(dataset_name, [4] * 10)
        self.assertFalse(resp.error)
        self.assertEqual(200, resp.response_code)
        resp = self.c.dslist()
        self.assertEqual(6, resp.data[0]['dimensions']['rows'])
        self.assertEqual(2, resp.data[0]['dimensions']['columns'])

