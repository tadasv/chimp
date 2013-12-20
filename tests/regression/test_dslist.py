import unittest
import subprocess
from time import sleep
import uuid

import pychimp
import regression_settings


class DSLISTCommandTestCase(unittest.TestCase):
    def setUp(self):
        self.proc = subprocess.Popen(regression_settings.chimp_location)
        sleep(1)
        self.c = pychimp.ChimpClient(regression_settings.chimp_host)

    def tearDown(self):
        self.c.close()
        self.proc.kill()

    def test_dslist(self):
        resp = self.c.dslist()
        self.assertEqual(200, resp.response_code)
        self.assertEqual(0, len(resp.data))

        dataset_name = str(uuid.uuid4())
        self.c.dsnew(dataset_name, 4)
        resp = self.c.dslist()
        self.assertEqual(200, resp.response_code)
        self.assertEqual(1, len(resp.data))
        self.assertEqual({
            'name': dataset_name,
            'dimensions': {
                'rows': 0,
                'columns': 4,
            }}, resp.data[0])

        dataset_name = str(uuid.uuid4())
        self.c.dsnew(dataset_name, 8)
        resp = self.c.dslist()
        self.assertEqual(200, resp.response_code)
        self.assertEqual(2, len(resp.data))
