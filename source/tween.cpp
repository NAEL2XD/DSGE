#include "tween.hpp"
#include <thread>
#include <chrono>
#include <algorithm>
#include <cmath>

namespace dsge {

template<typename T>
T Tween::lerp(T start, T end, float progress) {
    return static_cast<T>(start + (end - start) * progress);
}

u32 Tween::lerpColor(u32 start, u32 end, float progress) {
    u8 start_a = (start >> 24) & 0xFF;
    u8 start_r = (start >> 16) & 0xFF;
    u8 start_g = (start >> 8) & 0xFF;
    u8 start_b = start & 0xFF;
    
    u8 end_a = (end >> 24) & 0xFF;
    u8 end_r = (end >> 16) & 0xFF;
    u8 end_g = (end >> 8) & 0xFF;
    u8 end_b = end & 0xFF;
    
    return ((static_cast<u32>(lerp(start_a, end_a, progress)) << 24) |
            (static_cast<u32>(lerp(start_r, end_r, progress)) << 16) |
            (static_cast<u32>(lerp(start_g, end_g, progress)) << 8) |
            static_cast<u32>(lerp(start_b, end_b, progress)));
}

float Tween::applyEase(float progress, eases ease) {
    switch(ease) {
        case EASE_LINEAR: return progress;
        
        // Sine eases
        case EASE_SINE_IN: return 1 - std::cos((progress * M_PI) / 2);
        case EASE_SINE_OUT: return std::sin((progress * M_PI) / 2);
        case EASE_SINE_IN_OUT: return -(std::cos(M_PI * progress) - 1) / 2;
        
        // Quadratic eases
        case EASE_QUAD_IN: return progress * progress;
        case EASE_QUAD_OUT: return 1 - (1 - progress) * (1 - progress);
        case EASE_QUAD_IN_OUT: return progress < 0.5 ? 2 * progress * progress : 1 - std::pow(-2 * progress + 2, 2) / 2;
        
        // Cubic eases
        case EASE_CUBIC_IN: return progress * progress * progress;
        case EASE_CUBIC_OUT: return 1 - std::pow(1 - progress, 3);
        case EASE_CUBIC_IN_OUT: return progress < 0.5 ? 4 * progress * progress * progress : 1 - std::pow(-2 * progress + 2, 3) / 2;
        
        // Exponential eases
        case EASE_EXPO_IN:  return progress == 0 ? 0 : std::pow(2, 10 * progress - 10);
        case EASE_EXPO_OUT: return progress == 1 ? 1 : 1 - std::pow(2, -10 * progress);
        case EASE_EXPO_IN_OUT: return progress == 0 ? 0 : progress == 1 ? 1 : progress < 0.5 ? std::pow(2, 20 * progress - 10) / 2 : (2 - std::pow(2, -20 * progress + 10)) / 2;
        
        default: return progress;
    }
}

template<typename T>
void Tween::tween_impl(T& target, const std::map<std::string, float>& properties, float duration, eases ease) {
    std::thread([&target, properties, duration, ease]() {
        using namespace std::chrono;
        auto start_time = steady_clock::now();
        float elapsed = 0;
        
        // Store initial values
        std::map<std::string, float> start_values;
        u32 start_color = target.color;
        
        for (const auto& prop : properties) {
            if (prop.first == "x") start_values["x"] = target.x;
            else if (prop.first == "y") start_values["y"] = target.y;
            else if (prop.first == "scale.x") start_values["scale.x"] = target.scale.x;
            else if (prop.first == "scale.y") start_values["scale.y"] = target.scale.y;
            else if (prop.first == "color") start_values["color"] = static_cast<float>(target.color);
            else if (prop.first == "angle") start_values["angle"] = target.angle;
            else if (prop.first == "alpha") start_values["alpha"] = target.alpha;
        }

        // Tween loop
        while (elapsed < duration) {
            elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time).count() / 1000;
            float linear_progress = std::min(elapsed / duration, 1.0f);
            float eased_progress = applyEase(linear_progress, ease);

            for (const auto& prop : properties) {
                if (prop.first == "x") {
                    target.x = lerp(start_values["x"], prop.second, eased_progress);
                } else if (prop.first == "y") {
                    target.y = lerp(start_values["y"], prop.second, eased_progress);
                } else if (prop.first == "scale.x") {
                    target.scale.x = lerp(start_values["scale.x"], prop.second, eased_progress);
                } else if (prop.first == "scale.y") {
                    target.scale.y = lerp(start_values["scale.y"], prop.second, eased_progress);
                } else if (prop.first == "color") {
                    target.color = lerpColor(start_color, static_cast<u32>(prop.second), eased_progress);
                } else if (prop.first == "angle") {
                    target.angle = lerp(start_values["angle"], prop.second, eased_progress);
                }  else if (prop.first == "alpha") {
                    target.alpha = lerp(start_values["alpha"], prop.second, eased_progress);
                }
            }

            svcSleepThread(1666666); // ~60fps
        }

        // Ensure final values are exact
        for (const auto& prop : properties) {
            if (prop.first == "x") target.x = prop.second;
            else if (prop.first == "y") target.y = prop.second;
            else if (prop.first == "scale.x") target.scale.x = prop.second;
            else if (prop.first == "scale.y") target.scale.y = prop.second;
            else if (prop.first == "color") target.color = static_cast<u32>(prop.second);
            else if (prop.first == "angle") target.angle = prop.second;
            else if (prop.first == "alpha") target.alpha = prop.second;
        }
    }).detach();
}

void Tween::tween(dsge::Sprite& target, const std::map<std::string, float>& properties, float duration, eases ease) {
    tween_impl(target, properties, duration, ease);
}

void Tween::tween(dsge::Text& target, const std::map<std::string, float>& properties, float duration, eases ease) {
    tween_impl(target, properties, duration, ease);
}

// Explicit template instantiation
template void Tween::tween_impl<dsge::Sprite>(dsge::Sprite&, const std::map<std::string, float>&, float, eases);
template void Tween::tween_impl<dsge::Text>(dsge::Text&, const std::map<std::string, float>&, float, eases);

} // namespace dsge