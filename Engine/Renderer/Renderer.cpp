//
// Created by Owner on 1/9/2023.
//

#include "Renderer.hpp"

static uint32_t s_RendererGlobalID = 0;


Renderer::Renderer(uint32_t width, uint32_t height) : gameLoopValid(true), time(clock()) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG);
    window = SDL_CreateWindow(std::string("Window" + std::to_string(s_RendererGlobalID)).c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width,
                              height,
                              SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window,
                                  s_RendererGlobalID++,
                                  SDL_RENDERER_ACCELERATED);
}
Renderer::~Renderer() {
    IMG_Quit();
    SDL_Quit();
}

Event Renderer::pollEvents() {
    Event e{};

    // TODO: Make Minimization Use Less Resource Power
    if (e.event.type == SDL_QUIT) gameLoopValid = false;

    return e;
}

bool Renderer::gameLoopIsValid() const { return gameLoopValid; }

Texture Renderer::loadTexture(const char* path, int textureX, int textureY, int textureW, int textureH) {
    Texture texture {};

    for (size_t loop = 0; auto& look : *Texture::getGlobalTextures()) {
        if (look.path == path) {
            if (!look.loaded) {
                SDL_Surface* image = IMG_Load(look.path.c_str());
                if (image == nullptr) {
                    std::cerr << "Error Loading Image: " << SDL_GetError() << "\n";
                    return Texture(0,0,0,0,0);
                }
                look.texture = SDL_CreateTextureFromSurface(renderer, image);
                if (look.texture == nullptr) {
                    std::cerr << "Error Loading Image: " << SDL_GetError() << "\n";
                    return Texture(0,0,0,0,0);
                }
                SDL_FreeSurface(image);
                SDL_QueryTexture(look.texture, nullptr, nullptr, &look.rect.w, &look.rect.h);
                look.references++;
                look.print();
                Texture texture( loop, textureX, textureY,
                        (textureW < 0) ? look.rect.w : textureW,
                        (textureH < 0) ? look.rect.h : textureH );
                return texture;
            } else {
                texture.gTextureIndex = loop;
                texture.textureX = textureX;
                texture.textureY = textureY;
                texture.textureW = (textureW < 0) ? look.rect.w : textureW;
                texture.textureH = (textureH < 0) ? look.rect.h : textureH;
                look.references++;
                printf("Assigning Texture %zi Out Of %lli, References: %i\n", loop+1, (*Texture::getGlobalTextures()).size(), look.references);
                return texture;
            }
        }
        loop++;
    }

    SDL_Surface* image = IMG_Load(path);
    if (image == nullptr) {
        std::cerr << "Error Loading Image: " << SDL_GetError() << "\n";
        return Texture(0,0,0,0,0);
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, image);
    if (tex == nullptr) {
        std::cerr << "Error Loading Image: " << SDL_GetError() << "\n";
        return Texture(0,0,0,0,0);
    }
    SDL_FreeSurface(image);
    int width, height;
    SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);

    TextureLookUp lookUp {
            .path = path,
            .loaded = true,
            .texture = tex,
            .rect = {0,0,width,height},
            .references = 1,
    };
    (*Texture::getGlobalTextures()).push_back(lookUp);

    texture = Texture((*Texture::getGlobalTextures()).size()-1, textureX, textureY, (textureW < 0) ? width : textureW, (textureH < 0) ? height : textureH);
    printf("Creating New Texture %zi Out Of %lli, References: %i\n", (*Texture::getGlobalTextures()).size(), (*Texture::getGlobalTextures()).size(), lookUp.references);

    return texture;
}
void Renderer::unloadTexture(Texture& texture) {
    auto tex = (*Texture::getGlobalTextures())[texture.gTextureIndex];
    tex.references--;
    printf("Unloading Texture, References: %i\n", tex.references);
    if (tex.references <= 0) {
        SDL_DestroyTexture((*Texture::getGlobalTextures())[texture.gTextureIndex].texture);
        tex.loaded = false;
    }
    texture.gTextureIndex = -1;
}
template<typename RectType>
void Renderer::drawRect(RectType rect, std::optional<RectDrawProperties> props) requires concept_ConvertableToSDLRect<RectType> {
    if (props.has_value() && !props->filled)
        SDL_RenderDrawRect(renderer, &rect);
    else
        SDL_RenderFillRect(renderer, &rect);
}
void Renderer::drawRect(Rect &r, std::optional<Renderer::RectDrawProperties> props) {
    if (r._textures.empty()) {
        RenderDrawColor ogColor = getColor();
        setColor(r.col);
        drawRect(r.pos, props);
        setColor(ogColor);
    } else {
        TextureDrawProperties props {
                .rotation = r._rot,
                .rotationOrigin = r._rotOrigin,
                .flip = r._flip,
        };
        drawTexture(r.pos, r._textures[r._textureIndex], props);
    }
}

void Renderer::drawRectArray(std::vector<SDL_Rect> &rects, std::optional<RectDrawProperties> props) {
    if (props.has_value() && !props->filled)
        SDL_RenderDrawRects(renderer, rects.data(), (size_t)rects.size());
    else
        SDL_RenderFillRects(renderer, rects.data(), (size_t)rects.size());
}
void Renderer::drawTexture(SDL_Rect pos, Texture& texture, std::optional<TextureDrawProperties> props) {
    SDL_Rect texturePos = {texture.textureX, texture.textureY, texture.textureW, texture.textureH};

    if (props.has_value())
        SDL_RenderCopyEx(renderer,
                        (*Texture::getGlobalTextures())[texture.gTextureIndex].texture,
                         &texturePos,
                         &pos,
                         props.value().rotation,
                         &props.value().rotationOrigin,
                         props.value().flip);
    else
        SDL_RenderCopy(renderer,
                       (*Texture::getGlobalTextures())[texture.gTextureIndex].texture,
                       &texturePos,
                       &pos);
}

void Renderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
void Renderer::setColor(RenderDrawColor color) {
    setColor(color.r, color.g, color.b, color.a);
}
RenderDrawColor Renderer::getColor() {
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    return {r, g, b, a};
}

void Renderer::clear() {
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
void Renderer::present() {
    SDL_RenderPresent(renderer);
}


void drawObjectFromSceneView(Renderer& renderer, Rect& r, std::optional<Renderer::RectDrawProperties> props) {
    renderer.drawRect(getAABBFromSceneView(r), props);
}
void drawObjectFromSceneView(Renderer& renderer, SDL_Rect r, std::optional<Renderer::RectDrawProperties> props) {
    renderer.drawRect(getAABBFromSceneView(r), props);
}
void drawObjectFromSceneView(Renderer& renderer, SDL_Rect r, Texture& texture, std::optional<Renderer::TextureDrawProperties> props) {
    renderer.drawTexture(getAABBFromSceneView(r), texture, props);
}



