#include <gtest/gtest.h>
#include <cstring>
#include <iostream>
#include <msgpack.h>
#include <ch_protocol.h>

TEST(ch_response_message_t, init)
{
    ch_response_message_t msg;
    ASSERT_EQ(msg.code, CH_RESPONSE_CODE_OK);
}

TEST(ch_response_message_t, serialize)
{
    ch_response_message_t msg;
    msgpack_sbuffer *buf;

    buf = msg.serialize();
    ASSERT_TRUE(buf != NULL);
    ASSERT_EQ(3, buf->size);
    msgpack_sbuffer_free(buf);

    msg.code = CH_RESPONSE_CODE_USER_ERROR;
    buf = msg.serialize();
    ASSERT_TRUE(buf != NULL);
    ASSERT_EQ(5, buf->size);
    ASSERT_EQ(0, msg.error.size());

    msg.error = "message";
    buf = msg.serialize();
    ASSERT_TRUE(buf != NULL);
    ASSERT_EQ(12, buf->size);
    ASSERT_EQ(7, msg.error.size());
    msgpack_sbuffer_free(buf);
}


TEST(ch_dsnew_message_t, serialization)
{
    ch_message_dsnew_t msg_to_serialize;
    ch_message_dsnew_t msg_unserialized;
    msgpack_sbuffer *buffer;

    msg_to_serialize.name = "iris";
    msg_to_serialize.num_columns = 10;

    buffer = msg_to_serialize.serialize();
    ASSERT_TRUE(buffer != NULL);

    msgpack_unpacked msg;
    msgpack_unpacked_init(&msg);
    ASSERT_TRUE(msgpack_unpack_next(&msg, buffer->data, buffer->size, NULL));

    ASSERT_EQ(0, msg_unserialized.unserialize(&msg));
    ASSERT_EQ("iris", msg_unserialized.name);
    ASSERT_EQ(10, msg_unserialized.num_columns);

    msgpack_sbuffer_free(buffer);
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
