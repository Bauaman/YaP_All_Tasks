#include <iostream>
#include <stdexcept>
#include <cassert>
#include <string>

using namespace std;

class House {
    // Реализуйте самостоятельно
public:
    House(int l, int w, int h) :
        length_(l),
        width_(w),
        height_(h)
    {
    }

    int GetLength() const {
        return length_;
    }

    int GetWidth() const {
        return width_;
    }

    int GetHeight() const {
        return height_;
    }

private:
    int length_;
    int width_;
    int height_;
};

class Resources {
    // Реализуйте самостоятельно
public:
    Resources (int bricks_init) :
        bricks_count_(bricks_init)
    {
    }

    void TakeBricks(int bricks_qty) {
        if (bricks_qty > bricks_count_) {
            throw std::out_of_range("Недостаточно кирпичей");
        }
        if (bricks_qty < 0) {
            throw std::out_of_range("Отрицательное оличество кирпичей");
        }
        bricks_count_-=bricks_qty;
    }

    int GetBrickCount() const {
        return bricks_count_;
    }

private:
    int bricks_count_;
};

struct HouseSpecification {
    int length = 0;
    int width = 0;
    int height = 0;
};

class Builder {
    // Реализуйте самостоятельно
public:
    Builder (Resources& res) :
        available_bricks(res)
    {
    }

    House BuildHouse (HouseSpecification h_spec) {
        int l_num_bricks = h_spec.length*4;
        int w_num_bricks = h_spec.width*4;
        int rows = h_spec.height*8;
        int bricks_qty = 2*(l_num_bricks+w_num_bricks)*rows;
        try {
            available_bricks.TakeBricks(bricks_qty);
        }
        catch (out_of_range& e) {
                throw runtime_error("Нехватка кирпичей");
        }
        return {h_spec.length, h_spec.width, h_spec.height};
    }
private:
    Resources& available_bricks;

};

int main() {
    Resources resources{10000};
    Builder builder1{resources};
    Builder builder2{resources};

    House house1 = builder1.BuildHouse(HouseSpecification{12, 9, 3});
    assert(house1.GetLength() == 12);
    assert(house1.GetWidth() == 9);
    assert(house1.GetHeight() == 3);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House house2 = builder2.BuildHouse(HouseSpecification{8, 6, 3});
    assert(house2.GetLength() == 8);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House banya = builder1.BuildHouse(HouseSpecification{4, 3, 2});
    assert(banya.GetHeight() == 2);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    //House not_a_house = builder1.BuildHouse(HouseSpecification{20,20,5});
}