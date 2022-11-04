#include <iostream>
#include <string>

class Cislo {
private:
    int mCislo = 0;
public:
    Cislo() = default;
    explicit Cislo(int cislo) : mCislo(cislo) { }
    explicit Cislo(int cislo1, int cislo2) : mCislo(cislo1 * cislo2) { }
    int get_cislo() const {
        return mCislo;
    }

    Cislo operator+(const Cislo& other) const {
        return Cislo{this->mCislo + other.mCislo};
    }

    operator int() const {
        return mCislo;
    }

    bool operator<(const Cislo& b) {
        return mCislo < b.get_cislo();
    }

    auto operator<=>(const Cislo& b) {
        return mCislo <=> b.get_cislo();
    }

    Cislo(const Cislo& a) = delete;
    Cislo& operator=(const Cislo& a) = delete;

};

void vypis_cislo(Cislo c) {
    std::cout << "Cislo je " << c.get_cislo() << std::endl;
}

Cislo secti_cisla(const Cislo& c1, const Cislo& c2) {
    return c1 + c2;
}

class INamed {
public:
    virtual ~INamed() { std::cout << "destroing inamed" << std::endl; }
    virtual std::string get_name() const = 0;
};

class Pet : public INamed {
public:
    ~Pet() { std::cout << "destroing azor" << std::endl; }
    std::string get_name() const override { return "Azor"; }
};

class Bridge :public INamed {
public:
    ~Bridge() { std::cout << "destroing bridge" << std::endl; }
    std::string get_name() const override { return "Most"; }
};

int main() {

//    INamed* pet1 = new Pet();
//    Pet* pet2 = new Pet();
//    std::cout << pet1->get_name() << std::endl;
//    std::cout << pet2->get_name() << std::endl;
//
//    delete pet1;
//    delete pet2;

    Cislo a;
    Cislo b{45};

    int d = a + b;

    //vypis_cislo(a);
    //vypis_cislo(b);
    vypis_cislo(Cislo{01});
    vypis_cislo(secti_cisla(a, b));

    return EXIT_SUCCESS;
}

