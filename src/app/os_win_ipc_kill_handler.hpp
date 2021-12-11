#ifndef OS_WIN_IPC_KILL_HANDLER_HPP

#include <boost/interprocess/ipc/message_queue.hpp>
#include <process.h>

/*
    Thats why:
    https://github.com/SambaGodschynski/werckmeister/issues/101
*/


namespace app {
    namespace os {
        struct InterProcessMessageQueue {
            typedef int MessageType;
            std::string queueName;
            static const MessageType SIGINT_MESSAGE = 1;
            static const MessageType MaxMessages = 100;
            

            boost::interprocess::message_queue* _queue = nullptr;

            
            InterProcessMessageQueue() 
            {
                using namespace boost::interprocess;
                queueName = std::string("sheet_ipc_") + std::to_string(::_getpid());
                message_queue::remove(queueName.c_str());
                _queue = new message_queue
                (	  create_only
                    , queueName.c_str()      // name
                    , MaxMessages            // max message number
                    , sizeof(MessageType)    // max message size
                );
            }

            InterProcessMessageQueue(int pid)
            {
                using namespace boost::interprocess;
                queueName = std::string("sheet_ipc_") + std::to_string(pid);
                _queue = new message_queue(open_only, queueName.c_str());
            }

            bool sigintReceived() const {
                using namespace boost::interprocess;
                auto numMessages = _queue->get_num_msg();
                if (numMessages == 0) {
                    return false;
                }
                unsigned int priority;
                message_queue::size_type recvd_size;
                while(true) {
                    MessageType message = -1;
                    _queue->receive(&message, sizeof(MessageType), recvd_size, priority);
                    if (recvd_size != sizeof(MessageType)) {
                        return false;
                    }
                    if (message == SIGINT_MESSAGE) {
                        return true;
                    }
                }
            }

            void sendSigint() {
                _queue->send(&SIGINT_MESSAGE, sizeof(MessageType), 0);
            }

            ~InterProcessMessageQueue() 
            {
                using namespace boost::interprocess;
                message_queue::remove(queueName.c_str());
                delete _queue;
            }
        };
    }
}

#endif OS_WIN_IPC_KILL_HANDLER_HPP