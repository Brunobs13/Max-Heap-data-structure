#include "max_heap.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using maxheap::MaxHeap;

namespace {

std::vector<int> parse_values(std::stringstream& stream) {
    std::vector<int> values;
    int value = 0;
    while (stream >> value) {
        values.push_back(value);
    }
    return values;
}

void print_heap_array(const MaxHeap& heap) {
    const auto& arr = heap.array();
    std::cout << "heap=[";
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i + 1 < arr.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

void print_heap_tree(const MaxHeap& heap) {
    const auto& arr = heap.array();
    if (arr.empty()) {
        std::cout << "heap is empty\n";
        return;
    }

    std::size_t level = 0;
    std::size_t index = 0;
    while (index < arr.size()) {
        const std::size_t level_size = (std::size_t)1 << level;
        std::cout << "L" << level << ": ";
        for (std::size_t j = 0; j < level_size && index < arr.size(); ++j, ++index) {
            std::cout << std::setw(3) << arr[index] << ' ';
        }
        std::cout << '\n';
        ++level;
    }
}

void print_help() {
    std::cout
        << "Commands:\n"
        << "  insert <v1> <v2> ...\n"
        << "  delete\n"
        << "  peek\n"
        << "  print\n"
        << "  tree\n"
        << "  build <v1> <v2> ...\n"
        << "  resize <capacity>\n"
        << "  size\n"
        << "  capacity\n"
        << "  clear\n"
        << "  help\n"
        << "  exit\n";
}

}  // namespace

int main() {
    MaxHeap heap(15);
    std::string line;

    std::cout << "maxheap-cli ready. type 'help' for commands\n";
    while (std::getline(std::cin, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream parser(line);
        std::string command;
        parser >> command;

        if (command == "insert") {
            const auto values = parse_values(parser);
            if (values.empty()) {
                std::cout << "insert requires at least one integer\n";
                continue;
            }

            for (const int value : values) {
                if (!heap.insert(value)) {
                    std::cout << "insert rejected: heap is full\n";
                    break;
                }
            }
            print_heap_array(heap);
        } else if (command == "delete") {
            const auto removed = heap.delete_max();
            if (!removed.has_value()) {
                std::cout << "delete rejected: heap is empty\n";
                continue;
            }
            std::cout << "deleted=" << removed.value() << '\n';
            print_heap_array(heap);
        } else if (command == "peek") {
            const auto top = heap.peek_max();
            if (!top.has_value()) {
                std::cout << "heap is empty\n";
                continue;
            }
            std::cout << "max=" << top.value() << '\n';
        } else if (command == "print") {
            print_heap_array(heap);
        } else if (command == "tree") {
            print_heap_tree(heap);
        } else if (command == "build") {
            const auto values = parse_values(parser);
            if (!heap.build_from(values)) {
                std::cout << "build rejected: values exceed current capacity\n";
                continue;
            }
            print_heap_array(heap);
        } else if (command == "resize") {
            std::size_t new_capacity = 0;
            if (!(parser >> new_capacity)) {
                std::cout << "resize requires a positive integer capacity\n";
                continue;
            }
            if (!heap.resize_capacity(new_capacity)) {
                std::cout << "resize rejected: new capacity smaller than current size or zero\n";
                continue;
            }
            std::cout << "capacity=" << heap.capacity() << '\n';
        } else if (command == "size") {
            std::cout << "size=" << heap.size() << '\n';
        } else if (command == "capacity") {
            std::cout << "capacity=" << heap.capacity() << '\n';
        } else if (command == "clear") {
            heap.clear();
            std::cout << "heap cleared\n";
        } else if (command == "help") {
            print_help();
        } else if (command == "exit" || command == "quit") {
            break;
        } else {
            std::cout << "unknown command: " << command << '\n';
        }
    }

    return 0;
}
