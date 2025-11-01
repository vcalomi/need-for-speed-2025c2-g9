#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

class LevelCreator {
    public: 
        using Matrix = std::vector<std::vector<int>>;
        
        Matrix BuildLevelMatrix(const std::filesystem::path& file_path);
        void processDirectoryLevel(const std::string& directory_path);

        const std::vector<Matrix>& levels() const { return levels_matrices_; }
        
        void createLevelCollision(b2WorldId world, const std::vector<Matrix>& levels);

    private:
        std::vector<Matrix> levels_matrices_{4}; // 4 slots

        float ppm;
};







