#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
//#include <format>

//class ISerializable {
//    public:
//        virtual void serialize_to(std::ostream& stream) = 0;
//        virtual void deserialize_from(std::istream& stream) = 0;
//};
//
//class IPrintable {
//    public:
//        virtual void print_info(std::ostream& stream) = 0;
//};
//
//class BetterIntVector : public std::vector<int>, public ISerializable, public IPrintable {
//    public:
//        virtual void serialize_to(std::ostream& stream) override {}
//        virtual void deserialize_from(std::istream& stream) override {}
//
//};
//
//class A {
//    public:
//        virtual void fncA() { /*...*/ }
//        int a = 0;
//};
//
//class B : public A {
//    public:
//        virtual void fncB() { /*...*/ }
//        int b = 0;
//};
//
//class C : public A {
//    public:
//        virtual void fncC() { /*...*/ }
//        virtual void fncA() override { /*...*/ }
//        int c = 0;
//};

//#pragma pack(push, 1)
//
//struct FSHeader {
//    char magic[6];
//    int partition_size;
//    int file_count;
//};
//
//struct FSFileEntry {
//    char filename[15];
//    int type;
//    int size;
//};
//
//#pragma pack(pop)

class BasePacket {
    protected:
        virtual std::string serialize_body() = 0;

    public:
        virtual std::string serialize() {
            std::string body = serialize_body();

            // ups004

            std::ostringstream  oss;
            oss << "UPS";
//            oss << std::setw(3) << std::setfill('0') << body.length();
            oss << std::format("{:0>3}", body.length());

            oss << body;

            return oss.str();
        }
};

class LoginPacket : BasePacket {
    private:
        std::string mUsername, mPassword;

    protected:
        std::string serialize_body() override {

        }

    public:
        LoginPacket(const std::string& username, const std::string& password)
            : mUsername(username), mPassword(password) {}
};

int main(int argc, char** argv) {

    auto



//    std::cout << sizeof(FSHeader) << std::endl;

//    FSHeader hdr;
//    hdr.magic[0] = 'K';
//    hdr.magic[1] = 'I';
//    hdr.magic[2] = 'V';
//    hdr.magic[3] = 'C';
//    hdr.magic[4] = 'P';
//    hdr.magic[5] = 'P';
//    hdr.partition_size = 512 * 1024 * 1024;
//    hdr.file_count = 15;
//
//    std::ofstream disk("disk.bin", std::ios::out | std::ios::binary);
//
//    disk.write(reinterpret_cast<const char *>(&hdr), sizeof(FSHeader));

//    FSHeader hdr;
//    std::ifstream disk("disk.bin", std::ios::in | std::ios::binary);
//    disk.read(reinterpret_cast<char *>(&hdr), sizeof(FSHeader));
//
//    disk.seekg(0, std::ios::end);
//    auto pos = disk.tellg();
//    disk.seekg(0, std::ios::beg);


//    disk.seekg(sizeof(FSHeader) + 10 * sizeof(FSFileEntry));
//
//    std::cout << hdr.file_count << std::endl;

//
//    A* b = new B;
//    A* c = new C;
//
//    std::vector<A*> acka = {
//            new B,
//            new B,
//            new C,
//            new C,
//            new B
//    };
//
//    for (auto* ptr : acka) {
//        auto* bptr = dynamic_cast<B*>(ptr);
//
//        if (bptr) {
//            std::cout << bptr->b << std::endl;
//        }
//        else {
//            std::cout << "this is not B" << std::endl;
//        }
//
//    }

//    A* ca = static_cast<A*>(c);
//    B* cb = static_cast<B*>(c);

//    std::cout << a->a << std::endl;
//    std::cout << c->c << std::endl;



    return EXIT_SUCCESS;
}