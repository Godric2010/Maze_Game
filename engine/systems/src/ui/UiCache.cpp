#include "UiCache.hpp"

#include <stdexcept>

namespace Engine::Systems::UI {
    UiCache::UiCache() = default;

    UiCache::~UiCache() = default;

    void UiCache::RegisterTextElement(uint64_t entity) {
        if (m_text_cache.contains(entity)) {
            throw std::runtime_error("UI Cache::Text entity already registered");
        }
        m_text_cache.emplace(entity, TextElement{});
    }

    void UiCache::RegisterColorElement(uint64_t entity, ColorElement color_element) {
        if (m_button_cache.contains(entity)) {
            throw std::runtime_error("UI Cache::Button entity already registered");
        }
        m_button_cache.emplace(entity, color_element);
    }

    void UiCache::DeregisterTextElement(const uint64_t entity) {
        if (m_text_cache.contains(entity)) {
            m_text_cache.erase(entity);
            return;
        }
        throw std::runtime_error("UI Cache::TextEntity was not registered");
    }

    void UiCache::DeregisterColorElement(const uint64_t entity) {
        if (m_button_cache.contains(entity)) {
            m_button_cache.erase(entity);
            return;
        }
        throw std::runtime_error("UI Cache::ButtonEntity was not registered");
    }

    void UiCache::SetTextElementValue(const uint64_t entity, const TextElement text_element) {
        if (m_text_cache.contains(entity)) {
            m_text_cache[entity] = text_element;
            return;
        }
        throw std::runtime_error("UI Cache::TextElementValue was not registered");
    }

    void UiCache::SetColorElementValue(const uint64_t entity, ColorElement color_element) {
        if (m_button_cache.contains(entity)) {
            m_button_cache[entity] = color_element;
            return;
        }
        throw std::runtime_error("UI Cache::ButtonElementValue was not registered");
    }

    UiCache::TextElement &UiCache::GetTextElement(const uint64_t entity) {
        if (m_text_cache.contains(entity)) {
            return m_text_cache[entity];
        }
        throw std::runtime_error("UI Cache::TextElement was not registered");
    }

    UiCache::ColorElement &UiCache::GetColorElement(const uint64_t entity) {
        if (m_button_cache.contains(entity)) {
            return m_button_cache[entity];
        }
        throw std::runtime_error("UI Cache::ButtonElement was not registered");
    }
} // namespace
