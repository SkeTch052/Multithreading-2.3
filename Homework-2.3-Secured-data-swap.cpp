#include <iostream>
#include <mutex>
#include <thread>

class Data {
private:
    int data_;
public:
    Data(int value) : data_(value) {}

    std::mutex mut;
    int getData() const { return data_; }
    void setData(int value) { data_ = value; }
};

void printData(Data& data1, Data& data2) {
    std::cout << "Data1: " << data1.getData() << ", Data2: " << data2.getData() << std::endl;
}

void swap_with_lock(Data& data1, Data& data2) {
    std::lock(data1.mut, data2.mut);
    std::lock_guard<std::mutex> lock1(data1.mut, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(data2.mut, std::adopt_lock);

    int temp = data1.getData();
    data1.setData(data2.getData());
    data2.setData(temp);

    printData(data1, data2);
}

void swap_with_scoped_lock(Data& data1, Data& data2) {
    std::scoped_lock lock(data1.mut, data2.mut);

    int temp = data1.getData();
    data1.setData(data2.getData());
    data2.setData(temp);

    printData(data1, data2);
}

void swap_with_unique_lock(Data& data1, Data& data2) {
    std::unique_lock<std::mutex> lock1(data1.mut);
    std::unique_lock<std::mutex> lock2(data2.mut);

    int temp = data1.getData();
    data1.setData(data2.getData());
    data2.setData(temp);

    printData(data1, data2);
    lock1.unlock(); 
    lock2.unlock();
}

int main() {
    Data d1(10);
    Data d2(20);

    std::thread tr1 (swap_with_lock, std::ref(d1), std::ref(d2));
    std::thread tr2 (swap_with_scoped_lock, std::ref(d1), std::ref(d2));
    std::thread tr3 (swap_with_unique_lock, std::ref(d1), std::ref(d2));

    tr1.join();
    tr2.join();
    tr3.join();

    return 0;
}