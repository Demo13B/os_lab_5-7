#include <iostream>
#include "ZMQ.hpp"
#include "balancedTree.hpp"
#include "dictionaryNode.hpp"

void menu() {
    std::cout << "---------------------\n";
    std::cout << "Avaliable commands:\n";
    std::cout << "1. create <id>\n";
    std::cout << "2. exec <key>\n";
    std::cout << "3. ping <id>\n";
    std::cout << "4. kill <id>\n";
    std::cout << "5. exit\n";
    std::cout << "---------------------\n";
}

int main() {
    std::string command;
    DictionaryNode node(-1, -1, -1);
    std::string answer;
    BalancedTree tree;

    menu();

    while (std::cin >> command) {
        int id;

        if (command == "create") {
            std::cin >> id;
            if (tree.Exist(id)) {
                std::cout << "Error: Already exists\n";
            } else {
                while (true) {
                    int idParent = tree.FindID();
                    if (idParent == node.id) {
                        answer = node.create(id);
                        tree.AddInTree(id, idParent);
                        break;
                    } else {
                        std::string message = "create " + std::to_string(id);
                        answer = node.sendstring(message, idParent);
                        if (answer == "Error: Parent not found") {
                            tree.AvailabilityCheck(idParent);
                        } else {
                            tree.AddInTree(id, idParent);
                            break;
                        }
                    }
                }
                std::cout << answer << std::endl;
            }
        }

        else if (command == "exec") {
            std::cin >> id;
            if (!tree.Exist(id)) {
                std::cout << "Error: Node does not exist!\n";
            }

            std::string str;
            getline(std::cin, str);

            if (!tree.Exist(id)) {
                std::cout << "Error: Parent is not existed\n";
            } else {
                std::string message = "exec " + str;
                answer = node.sendstring(message, id);
                std::cout << answer << std::endl;
            }
        }

        else if (command == "ping") {
            std::cin >> id;
            if (!tree.Exist(id)) {
                std::cout << "Ok: 0\n";
            } else if (node.left_id == id || node.right_id == id) {
                answer = node.ping(id);
                std::cout << answer << std::endl;
            } else {
                std::string message = "ping " + std::to_string(id);
                answer = node.sendstring(message, id);
                std::cout << answer << std::endl;
            }
        }

        else if (command == "kill") {
            std::cin >> id;
            std::string message = "kill";
            if (!tree.Exist(id)) {
                std::cout << "Error: Parent is not existed\n";
            } else {
                answer = node.sendstring(message, id);
                if (answer != "Error: Parent not found") {
                    tree.RemoveFromRoot(id);
                    if (id == node.left_id) {
                        node.left_id = -2;
                        unbind(node.left, node.left_port);
                        answer = "Ok";
                    } else if (id == node.right_id) {
                        node.right_id = -2;
                        unbind(node.right, node.right_port);
                        answer = "Ok";
                    } else {
                        message = "clear " + std::to_string(id);
                        answer = node.sendstring(message, stoi(answer));
                    }
                    std::cout << answer << std::endl;
                }
            }
        }

        else if (command == "exit") {
            node.kill();
            std::cout << "Exited successfully" << std::endl;
            return 0;
        }

        else {
            std::cout << "Please enter correct command!\n\n";
            menu();
        }

        std::cout << "---------------------\n";
    }
}