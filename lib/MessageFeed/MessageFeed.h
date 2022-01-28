#include <Arduino.h>

class MessageFeed
{
public:
    String feed[5] = {
        "",
        "",
        "",
        "",
        ""};
    int total_messages = 0;
    void push(String message)
    {
        // check if messages exeeds 5 (feed array length)
        if (total_messages >= 5)
        {
            // if so, move strings in feed of index 1 to 4 to 0 to 3
            for (int i = 1; i < 5; i++)
            {
                feed[i - 1] = feed[i];
            }

            // and add message to index 4
            feed[4] = message;
        }
        else
        {
            // else add message to the next empty index
            feed[total_messages] = message;
        }
    };
}