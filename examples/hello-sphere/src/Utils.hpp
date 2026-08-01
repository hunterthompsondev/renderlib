#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <stb_image.h>

inline std::vector<uint8_t> ReadFileToBytes(const std::string &path)
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<uint8_t> buffer(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char *>(buffer.data()), fileSize);

    return buffer;
}

struct LoadedImage
{
    std::vector<uint8_t> pixels;
    int width = 0;
    int height = 0;
};

inline LoadedImage LoadImageRGBA8(const std::string &path)
{
    int width, height, channels;
    stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha); // force 4 channels
    if (!data)
    {
        throw std::runtime_error("Failed to load image: " + path);
    }

    LoadedImage result;
    result.width = width;
    result.height = height;
    result.pixels.assign(data, data + (width * height * 4));

    stbi_image_free(data);
    return result;
}

struct VertexKey
{
    uint32_t position;
    uint32_t texcoord;
    uint32_t normal;

    bool operator==(const VertexKey &) const = default;
};

struct VertexKeyHash
{
    size_t operator()(const VertexKey &k) const
    {
        return (static_cast<size_t>(k.position) << 42) ^ (static_cast<size_t>(k.texcoord) << 21) ^
               static_cast<size_t>(k.normal);
    }
};

inline void LoadOBJ(const std::string &path, std::vector<float> &vertices, std::vector<uint16_t> &indices)
{
    vertices.clear();
    indices.clear();

    std::ifstream file(path);

    if (!file)
        throw std::runtime_error("Failed to open OBJ.");

    std::vector<float> positions;
    std::vector<float> texcoords;
    std::vector<float> normals;

    std::unordered_map<VertexKey, uint16_t, VertexKeyHash> vertexMap;

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        std::string type;
        ss >> type;

        if (type == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;

            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }
        else if (type == "vt")
        {
            float u, v;
            ss >> u >> v;

            texcoords.push_back(u);
            texcoords.push_back(v);
        }
        else if (type == "vn")
        {
            float x, y, z;
            ss >> x >> y >> z;

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
        else if (type == "f")
        {
            for (int i = 0; i < 3; ++i)
            {
                std::string token;
                ss >> token;

                std::replace(token.begin(), token.end(), '/', ' ');

                std::stringstream face(token);

                uint32_t p, t, n;
                face >> p >> t >> n;

                --p;
                --t;
                --n;

                VertexKey key{p, t, n};

                auto it = vertexMap.find(key);

                if (it != vertexMap.end())
                {
                    indices.push_back(it->second);
                    continue;
                }

                if ((vertices.size() / 8) >= UINT16_MAX)
                    throw std::runtime_error("Mesh exceeds uint16_t index limit.");

                uint16_t index = static_cast<uint16_t>(vertices.size() / 8);

                vertexMap[key] = index;
                indices.push_back(index);

                // Position
                vertices.push_back(positions[p * 3 + 0]);
                vertices.push_back(positions[p * 3 + 1]);
                vertices.push_back(positions[p * 3 + 2]);

                // UV
                vertices.push_back(texcoords[t * 2 + 0]);
                vertices.push_back(texcoords[t * 2 + 1]);

                // Normal
                vertices.push_back(normals[n * 3 + 0]);
                vertices.push_back(normals[n * 3 + 1]);
                vertices.push_back(normals[n * 3 + 2]);
            }
        }
    }
}
