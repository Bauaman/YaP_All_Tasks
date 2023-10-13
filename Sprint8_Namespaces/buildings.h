#include <vector>

namespace buildings {
    struct Materials {};

    Materials GetMaterials();

    class Skyscraper {
        struct Floor {
            /* 1 */ Materials materials;
            int number;
        };

        Floor BuildFloor(/* 2 */ Materials materials);
        void BuildRoof(Materials materials);
        int GetMaterials(Materials source);

        bool IsComplete() const {
            return static_cast<int>(floors_.size()) < total_floors_;
        }

    public:
        void Build() {
            while (!IsComplete()) {
                auto materials = /* 3 */ buildings::GetMaterials();
                floors_.push_back(BuildFloor(std::move(materials)));
            }
        }

    private:
        int total_floors_ = 100;
        std::vector<Floor> floors_;
        /* 4 */ Materials roof_;
    };
} 