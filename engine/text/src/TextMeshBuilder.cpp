#include "TextMeshBuilder.hpp"

namespace Engine::Text {
    TextMesh TextMeshBuilder::GenerateTextMesh(const TextLayout &layout) {
        TextMesh text_mesh{};

        const auto &quads = layout.glyph_data;
        if (quads.empty()) {
            return text_mesh;
        }

        text_mesh.vertices.reserve(quads.size() * 4);
        text_mesh.indices.reserve(quads.size() * 6);

        int index_base = 0;
        for (const auto &quad: quads) {
            // Vertex order -> ccw
            // v0-------v1
            // |        |
            // |        |
            // v3------v2

            // skip space characters
            if (quad.x1 - quad.x0 == 0.0f) {
                continue;
            }

            TextMeshVertex v0{};
            v0.x = quad.x0;
            v0.y = quad.y0;
            v0.u = quad.u0;
            v0.v = quad.v0;

            TextMeshVertex v1{};
            v1.x = quad.x1;
            v1.y = quad.y0;
            v1.u = quad.u1;
            v1.v = quad.v0;

            TextMeshVertex v2{};
            v2.x = quad.x1;
            v2.y = quad.y1;
            v2.u = quad.u1;
            v2.v = quad.v1;

            TextMeshVertex v3{};
            v3.x = quad.x0;
            v3.y = quad.y1;
            v3.u = quad.u0;
            v3.v = quad.v1;

            text_mesh.vertices.push_back(v0);
            text_mesh.vertices.push_back(v1);
            text_mesh.vertices.push_back(v2);
            text_mesh.vertices.push_back(v3);

            text_mesh.indices.push_back(index_base + 0);
            text_mesh.indices.push_back(index_base + 1);
            text_mesh.indices.push_back(index_base + 2);

            text_mesh.indices.push_back(index_base + 0);
            text_mesh.indices.push_back(index_base + 2);
            text_mesh.indices.push_back(index_base + 3);

            index_base += 4;
        }

        return text_mesh;
    }
} // namespace
