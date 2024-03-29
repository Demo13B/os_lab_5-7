#include "balancedTree.hpp"
#include "dictionaryNode.hpp"
#include "ZMQ.hpp"

int main(int argc, char* argv[]) {
    DictionaryNode node(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

    while (true) {
        std::string message;
        std::string command;
        message = receive_message(node.parent);
        std::istringstream request(message);
        request >> command;

        if (command == "pid") {
            std::string answer = std::to_string(getpid());
            send_message(node.parent, answer);
        }

        else if (command == "ping") {
            int child;
            request >> child;
            std::string answer = node.ping(child);
            send_message(node.parent, answer);
        }

        else if (command == "create") {
            int child;
            request >> child;
            std::string answer = node.create(child);
            send_message(node.parent, answer);
        }

        else if (command == "exec") {
            std::string str;
            getline(request, str);
            std::string answer = node.exec(str);
            send_message(node.parent, answer);
        }

        else if (command == "kill") {
            std::string answer = node.kill();
            send_message(node.parent, answer);
            disconnect(node.parent, node.parent_port);
            node.parent.close();
            break;
        }
    }
    return 0;
}