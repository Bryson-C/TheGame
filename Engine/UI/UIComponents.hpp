//
// Created by Owner on 5/10/2023.
//

#ifndef SDL_UICOMPONENTS_HPP
#define SDL_UICOMPONENTS_HPP

#include <string>
#include <vector>

#include "../../lib/yaml/Yaml.hpp"
#include "../../Engine/Utility/Rect/Rect.hpp"
#include "../../Engine/Renderer/Renderer.hpp"


struct ComponentRenderData {
    // Component Style Details
    v4<int> pos = {0, 0, 50, 50};
    v2<int> padding = {5, 5};
    v3<int> color = {60, 60, 60};
    bool useBorder = false;
    v3<int> borderColor = {200, 200, 200};
    // Drawing
    void drawFrom(Renderer& renderer, int x, int y) const {
        auto col = renderer.getColor();
        renderer.setColor(color.x, color.y, color.z, 255);
        SDL_Rect box = {x + pos.x, y + pos.y, pos.z, pos.w};
        renderer.drawRect(box);
        if (useBorder) {
            renderer.setColor(borderColor.x, borderColor.y, borderColor.z, 255);
            renderer.drawRect(box, {{.filled = false}});
        }
        renderer.setColor(col);
    }
    void draw(Renderer& renderer) const {
        drawFrom(renderer, padding.x, padding.y);
    }

    // Style Functions
    inline void setSize(int width, int height) { pos.z = width; pos.w = height; }
    inline v2<int> getSize() { return {pos.z, pos.w}; }

    inline void setPosition(int x, int y) { pos.x = x; pos.y = y; }
    inline v2<int> getPosition() { return {pos.x, pos.y}; }

    inline void showBorder() { useBorder = true; }
    inline void hideBorder() { useBorder = false; }

    inline void setPadding(int x, int y) { padding.x = x; padding.y = y; }
    inline v2<int> getPadding() { return padding; }
};


class Button {
private:
    ComponentRenderData renderData{};
    friend class UIComponent;
    friend class Panel;
public:
    Button() = default;
    Button& showBorder() { renderData.showBorder(); return *this; }
    Button& hideBorder() { renderData.hideBorder(); return *this; }
    Button& setPosition(int x, int y) { renderData.setPosition(x,y); return *this; }
    v2<int> getPosition() { return renderData.getPosition(); }
    Button& setSize(int width, int height) { renderData.setSize(width, height); return *this; }
    v2<int> getSize() { return renderData.getSize(); }
    Button& setPadding(int x, int y) { renderData.setPadding(x, y); return *this; }
    v2<int> getPadding() { return renderData.getPadding(); }

    void draw(Renderer& renderer) const { renderData.draw(renderer); }
    void drawFrom(Renderer& renderer, int x, int y) const { renderData.drawFrom(renderer, x, y); }
};

class TextFeild {
private:
    ComponentRenderData renderData{};
    friend class UIComponent;
    friend class Panel;
public:
    TextFeild() = default;
    TextFeild& showBorder() { renderData.showBorder(); return *this; }
    TextFeild& hideBorder() { renderData.hideBorder(); return *this; }
    TextFeild& setPosition(int x, int y) { renderData.setPosition(x,y); return *this; }
    v2<int> getPosition() { return renderData.getPosition(); }
    TextFeild& setSize(int width, int height) { renderData.setSize(width, height); return *this; }
    v2<int> getSize() { return renderData.getSize(); }
    TextFeild& setPadding(int x, int y) { renderData.setPadding(x, y); return *this; }
    v2<int> getPadding() { return renderData.getPadding(); }
/*
    std::string text;
    void setText(std::string str) { text = str; }
    void drawText(Renderer& renderer, FontRenderer& fontRenderer) const { fontRenderer.drawText(renderer, text, renderData.pos.x, renderData.pos.y); }
    void drawTextFrom(Renderer& renderer, FontRenderer& fontRenderer, int x, int y) const { fontRenderer.drawText(renderer, text, renderData.pos.x + x, renderData.pos.y + y); }
*/
    void draw(Renderer& renderer) const { renderData.draw(renderer); }
    void drawFrom(Renderer& renderer, int x, int y) const { renderData.drawFrom(renderer, x, y); }

};

struct UIComponent {
    enum class Type {
        Unknown,
        Button,
        TextFeild
    } type;
    ComponentRenderData* pRenderData;
    union Component {
        Button button;
        TextFeild textFeild;
    } component{};
    UIComponent() = default;
    UIComponent(Button& button) : type(Type::Button), pRenderData(&button.renderData) { component.button = button; }
    UIComponent(TextFeild& textFeild) : type(Type::TextFeild), pRenderData(&textFeild.renderData) { component.textFeild = textFeild; }

    void draw(Renderer& renderer) const {
        switch (type) {
            case Type::Unknown: fprintf(stderr, "Cannot Draw For Unknown Component Type\n"); break;
            case Type::Button: component.button.draw(renderer); break;
            case Type::TextFeild: component.textFeild.draw(renderer); break;
            default: fprintf(stderr, "Cannot Draw For Component Type %i\n", (int)type); break;
        }
    }
    void drawFrom(Renderer& renderer, int x, int y) const {
        switch (type) {
            case Type::Unknown: fprintf(stderr, "Cannot Draw For Unknown Component Type\n"); break;
            case Type::Button: component.button.drawFrom(renderer, x, y); break;
            case Type::TextFeild: component.textFeild.drawFrom(renderer, x, y); break;
            default: fprintf(stderr, "Cannot Draw For Component Type %i\n", (int)type); break;
        }
    }
};



class Panel {
private:
    ComponentRenderData renderData{};
    std::vector<UIComponent> components;
public:
    Panel() = default;
    void addComponent(Button& button) { UIComponent comp{button}; components.push_back(comp); }
    void addComponent(TextFeild& textFeild) { UIComponent comp{textFeild}; components.push_back(comp); }

    void draw(Renderer& renderer) {
        renderData.draw(renderer);
        for (const auto& comp : components) {
            v2<int> pos {(renderData.pos.x + renderData.padding.x), (renderData.pos.y + renderData.padding.y)};
            comp.drawFrom(renderer, pos.x + comp.pRenderData->padding.x, pos.y + comp.pRenderData->padding.y);
        }
    }

    Panel& showBorder() { renderData.showBorder(); return *this; }
    Panel& hideBorder() { renderData.hideBorder(); return *this; }
    Panel& setPosition(int x, int y) { renderData.setPosition(x,y); return *this; }
    v2<int> getPosition() { return renderData.getPosition(); }
    Panel& setSize(int width, int height) { renderData.setSize(width, height); return *this; }
    v2<int> getSize() { return renderData.getSize(); }
    Panel& setPadding(int x, int y) { renderData.setPadding(x, y); return *this; }
    v2<int> getPadding() { return renderData.getPadding(); }
};


#endif //SDL_UICOMPONENTS_HPP
