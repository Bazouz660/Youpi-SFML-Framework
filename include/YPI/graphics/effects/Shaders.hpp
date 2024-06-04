#pragma once

#include "../../../lib_headers/common.hpp"
#include "../../math/Vector2.hpp"

namespace ypi::effect
{

    // gaussian blur shader
    // texture: the texture to blur
    // resolution: the resolution of the texture
    static sf::Shader* getBoxBlur(Vector2u resolution)
    {
        bool loaded = false;
        static sf::Shader* shader = new sf::Shader();
        if (!loaded)
        {
            shader->loadFromMemory(
                R"(
                    uniform sampler2D texture;
                    uniform vec2 resolution;
                    void main()
                    {
                        vec2 offset[9];
                        offset[0] = vec2(-1.0, -1.0);
                        offset[1] = vec2(0.0, -1.0);
                        offset[2] = vec2(1.0, -1.0);
                        offset[3] = vec2(-1.0, 0.0);
                        offset[4] = vec2(0.0, 0.0);
                        offset[5] = vec2(1.0, 0.0);
                        offset[6] = vec2(-1.0, 1.0);
                        offset[7] = vec2(0.0, 1.0);
                        offset[8] = vec2(1.0, 1.0);
                        vec4 sum = vec4(0.0);
                        float totalAlpha = 0.0;
                        for (int i = 0; i < 9; i++)
                        {
                            vec4 tex = texture2D(texture, gl_TexCoord[0].xy + offset[i] / resolution);
                            sum += tex * tex.a;
                            totalAlpha += tex.a;
                        }
                        gl_FragColor = sum / totalAlpha;
                    }
                )",
                sf::Shader::Fragment
            );
            loaded = true;
            shader->setUniform("texture", sf::Shader::CurrentTexture);
        }
        shader->setUniform("resolution", sf::Glsl::Vec2(resolution.x, resolution.y));
        return shader;
    }

    // fade borders shader, used to fade the borders of a texture (decaying alpha)
    // texture: the texture to fade
    // borderSize: the offset of the fade (0.0 to 1.0)
static sf::Shader* getFadeBorders(float offsetX, float offsetY) {
  bool loaded = false;
  static sf::Shader* shader = new sf::Shader();

  if (!loaded) {
    shader->loadFromMemory(
      R"(
        uniform sampler2D texture;
        uniform float offsetX;
        uniform float offsetY;

        void main() {
          vec2 uv = gl_TexCoord[0].xy;

          // Distance to edges
          float distX = smoothstep(0.0, offsetX, uv.x);
          float distLeft = smoothstep(0.0, offsetX, 1.0 - uv.x);
          float distY = smoothstep(0.0, offsetY, uv.y);
          float distTop = smoothstep(0.0, offsetY, 1.0 - uv.y);

          // Distance to corners
          float distTL = smoothstep(0.0, offsetY, 1.0 - uv.y) * smoothstep(0.0, offsetX, 1.0 - uv.x); // Top Left
          float distTR = smoothstep(0.0, offsetY, 1.0 - uv.y) * smoothstep(0.0, offsetX, uv.x); // Top Right
          float distBL = smoothstep(0.0, offsetY, uv.y) * smoothstep(0.0, offsetX, 1.0 - uv.x); // Bottom Left
          float distBR = smoothstep(0.0, offsetY, uv.y) * smoothstep(0.0, offsetX, uv.x); // Bottom Right

          // Calculate the final distance value
          float finalDist = min(min(distX, distLeft), min(distY, distTop));
          finalDist = min(finalDist, min(min(distTL, distTR), min(distBL, distBR)));

          // Fade color based on final distance
          vec4 texColor = texture2D(texture, uv);
          texColor *= finalDist;

          gl_FragColor = texColor;
        }
      )",
      sf::Shader::Fragment
    );
    loaded = true;
    shader->setUniform("texture", sf::Shader::CurrentTexture);
  }

  shader->setUniform("offsetX", offsetX);
  shader->setUniform("offsetY", offsetY);
  return shader;
}

} // namespace ypi::effect
/* 

float offset_x = 0.5;
float offset_y = 0.01;

struct Distances {
    float x_distance;
    float y_distance;
};

Distances man_dist_to_nearest_wall(vec2 uv)
{
    float aspect = (iResolution.y/iResolution.x);
    float x_distance = uv.x < 1.0 - uv.x ? uv.x : 1.0 - uv.x;
    float y_distance = uv.y * aspect < (1.0 - uv.y) * aspect ? uv.y * aspect: (1.0 - uv.y) * aspect;
    return Distances(x_distance, y_distance);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 texCoords = fragCoord.xy / iResolution.xy;
    vec4 texColor = texture(iChannel0, texCoords);

    Distances dist = man_dist_to_nearest_wall (texCoords);
    float dist_x = smoothstep(0.f, offset_x, dist.x_distance);
    float dist_y = smoothstep(0.f, offset_y, dist.y_distance);

    texColor *= dist_x * dist_y;

    fragColor = texColor;
}
 */