#include "effects.h"
#include <cmath>
#include <random>
#include <algorithm>

Effects::Effects() {
    Reset();
}

void Effects::Reset() {
    particles.clear();
    lineClearAnims.clear();
    landingEffect.active = false;
    screenShake = {0, 0, 0};
    comboDisplay.active = false;
}

// ==================== PARTICLE SYSTEM ====================

void Effects::AddParticles(Vector2 position, Color color, int count) {
    if (particles.size() >= MAX_PARTICLES) return;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angle(0, 2 * 3.14159f);
    std::uniform_real_distribution<float> speed(50, 200);
    std::uniform_real_distribution<float> size(2, 6);
    std::uniform_real_distribution<float> lifetime(0.5f, 1.5f);
    
    for (int i = 0; i < count; i++) {
        if (particles.size() >= MAX_PARTICLES) break;
        
        Particle p;
        p.position = position;
        
        float a = angle(gen);
        float s = speed(gen);
        p.velocity = {(float)cos(a) * s, (float)sin(a) * s};
        
        p.color = color;
        p.lifetime = 0;
        p.maxLifetime = lifetime(gen);
        p.size = size(gen);
        p.active = true;
        
        particles.push_back(p);
    }
}

void Effects::AddBlockLandParticles(std::vector<Position> positions, Color blockColor) {
    for (const auto& pos : positions) {
        Vector2 particlePos = {
            pos.column * 30.0f + 26.0f,  // Center of cell
            pos.row * 30.0f + 26.0f
        };
        AddParticles(particlePos, blockColor, 8);
    }
}

void Effects::AddLineClearExplosion(int row) {
    // Create explosion particles along the cleared row
    for (int col = 0; col < 10; col++) {
        Vector2 position = {
            col * 30.0f + 26.0f,
            row * 30.0f + 26.0f
        };
        
        // Random colors for explosion
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> colorDist(0, 3);
        
        Color explosionColors[] = {YELLOW, ORANGE, RED, WHITE};
        Color color = explosionColors[colorDist(gen)];
        
        AddParticles(position, color, 15);
    }
}

void Effects::AddLevelUpEffect(Vector2 position) {
    // Create burst of particles
    for (int i = 0; i < 50; i++) {
        AddParticles(position, GOLD, 1);
    }
}

// ==================== LINE CLEAR ANIMATION ====================

void Effects::StartLineClearAnimation(std::vector<int> rows) {
    for (int row : rows) {
        LineClearAnimation anim;
        anim.row = row;
        anim.progress = 0.0f;
        anim.flashProgress = 0.0f;
        anim.active = true;
        lineClearAnims.push_back(anim);
        
        // Add explosion particles
        AddLineClearExplosion(row);
    }
}

bool Effects::HasActiveLineClearAnimation() {
    for (const auto& anim : lineClearAnims) {
        if (anim.active) return true;
    }
    return false;
}

// ==================== LANDING EFFECT ====================

void Effects::StartLandingEffect(std::vector<Position> positions, Color color) {
    landingEffect.positions = positions;
    landingEffect.color = color;
    landingEffect.progress = 0.0f;
    landingEffect.active = true;
    
    // Add particles
    AddBlockLandParticles(positions, color);
}

// ==================== COMBO DISPLAY ====================

void Effects::StartComboDisplay(int combo, Vector2 position) {
    if (combo < 2) return; // Only show for combos of 2+
    
    comboDisplay.comboCount = combo;
    comboDisplay.position = position;
    comboDisplay.lifetime = 0.0f;
    comboDisplay.scale = 0.5f;
    comboDisplay.active = true;
}

// ==================== SCREEN SHAKE ====================

void Effects::StartScreenShake(float intensity, float duration) {
    screenShake.intensity = intensity;
    screenShake.duration = duration;
    screenShake.timer = 0.0f;
}

Vector2 Effects::GetScreenShakeOffset() {
    if (screenShake.timer >= screenShake.duration) {
        return {0, 0};
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    float progress = screenShake.timer / screenShake.duration;
    float currentIntensity = screenShake.intensity * (1.0f - progress);
    
    std::uniform_real_distribution<float> dis(-currentIntensity, currentIntensity);
    
    return {dis(gen), dis(gen)};
}

// ==================== UPDATE ====================

void Effects::Update(float deltaTime) {
    UpdateParticles(deltaTime);
    UpdateLineClearAnimations(deltaTime);
    UpdateLandingEffect(deltaTime);
    UpdateScreenShake(deltaTime);
    UpdateComboDisplay(deltaTime);
}

void Effects::UpdateParticles(float deltaTime) {
    for (auto& p : particles) {
        if (!p.active) continue;
        
        p.lifetime += deltaTime;
        p.position.x += p.velocity.x * deltaTime;
        p.position.y += p.velocity.y * deltaTime;
        
        // Gravity
        p.velocity.y += 300.0f * deltaTime;
        
        // Fade out
        if (p.lifetime >= p.maxLifetime) {
            p.active = false;
        }
    }
    
    // Remove inactive particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return !p.active; }),
        particles.end()
    );
}

void Effects::UpdateLineClearAnimations(float deltaTime) {
    for (auto& anim : lineClearAnims) {
        if (!anim.active) continue;
        
        anim.progress += deltaTime / LINE_CLEAR_DURATION;
        anim.flashProgress += deltaTime * 10.0f; // Fast flash
        
        if (anim.progress >= 1.0f) {
            anim.active = false;
        }
    }
    
    // Remove completed animations
    lineClearAnims.erase(
        std::remove_if(lineClearAnims.begin(), lineClearAnims.end(),
            [](const LineClearAnimation& a) { return !a.active; }),
        lineClearAnims.end()
    );
}

void Effects::UpdateLandingEffect(float deltaTime) {
    if (!landingEffect.active) return;
    
    landingEffect.progress += deltaTime / LANDING_DURATION;
    
    if (landingEffect.progress >= 1.0f) {
        landingEffect.active = false;
    }
}

void Effects::UpdateScreenShake(float deltaTime) {
    if (screenShake.timer < screenShake.duration) {
        screenShake.timer += deltaTime;
    }
}

void Effects::UpdateComboDisplay(float deltaTime) {
    if (!comboDisplay.active) return;
    
    comboDisplay.lifetime += deltaTime;
    
    // Scale animation (bounce in)
    if (comboDisplay.lifetime < 0.3f) {
        comboDisplay.scale = 0.5f + (comboDisplay.lifetime / 0.3f) * 0.5f;
    } else {
        comboDisplay.scale = 1.0f;
    }
    
    // Float upward
    comboDisplay.position.y -= 30.0f * deltaTime;
    
    if (comboDisplay.lifetime >= COMBO_DURATION) {
        comboDisplay.active = false;
    }
}

// ==================== DRAW ====================

void Effects::Draw() {
    DrawParticles();
    DrawLineClearAnimations();
    DrawLandingEffect();
    DrawComboDisplay();
}

void Effects::DrawParticles() {
    for (const auto& p : particles) {
        if (!p.active) continue;
        
        // Fade out alpha
        float alpha = 1.0f - (p.lifetime / p.maxLifetime);
        Color c = p.color;
        c.a = (unsigned char)(255 * alpha);
        
        DrawCircleV(p.position, p.size, c);
        
        // Add glow effect for larger particles
        if (p.size > 4) {
            Color glowColor = c;
            glowColor.a = (unsigned char)(128 * alpha);
            DrawCircleV(p.position, p.size + 2, glowColor);
        }
    }
}

void Effects::DrawLineClearAnimations() {
    for (const auto& anim : lineClearAnims) {
        if (!anim.active) continue;
        
        int row = anim.row;
        
        // Flash effect (white flash)
        float flash = sin(anim.flashProgress) * 0.5f + 0.5f;
        if (anim.progress < 0.3f) {
            Color flashColor = WHITE;
            flashColor.a = (unsigned char)(255 * flash * (1.0f - anim.progress / 0.3f));
            
            DrawRectangle(11, row * 30 + 11, 300, 29, flashColor);
        }
        
        // Fade out effect
        if (anim.progress > 0.3f) {
            float fadeProgress = (anim.progress - 0.3f) / 0.7f;
            Color fadeColor = YELLOW;
            fadeColor.a = (unsigned char)(200 * (1.0f - fadeProgress));
            
            // Draw from center outward
            float halfWidth = 150 * (1.0f - fadeProgress);
            DrawRectangle(11 + 150 - halfWidth, row * 30 + 11, 
                         halfWidth * 2, 29, fadeColor);
        }
    }
}

void Effects::DrawLandingEffect() {
    if (!landingEffect.active) return;
    
    float alpha = 1.0f - landingEffect.progress;
    Color effectColor = landingEffect.color;
    effectColor.a = (unsigned char)(150 * alpha);
    
    for (const auto& pos : landingEffect.positions) {
        // Draw expanding circle
        float radius = 15 * landingEffect.progress;
        Vector2 center = {
            pos.column * 30.0f + 26.0f,
            pos.row * 30.0f + 26.0f
        };
        
        DrawCircleV(center, radius, effectColor);
    }
}

void Effects::DrawComboDisplay() {
    if (!comboDisplay.active) return;
    
    float alpha = 1.0f;
    if (comboDisplay.lifetime > COMBO_DURATION - 0.5f) {
        // Fade out in last 0.5 seconds
        alpha = 1.0f - ((comboDisplay.lifetime - (COMBO_DURATION - 0.5f)) / 0.5f);
    }
    
    // Draw combo text with outline
    const char* text = TextFormat("COMBO x%d!", comboDisplay.comboCount);
    
    int fontSize = (int)(40 * comboDisplay.scale);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 2);
    
    Vector2 position = {
        comboDisplay.position.x - textSize.x / 2,
        comboDisplay.position.y
    };
    
    // Outline (black)
    for (int dx = -2; dx <= 2; dx += 2) {
        for (int dy = -2; dy <= 2; dy += 2) {
            Color outlineColor = BLACK;
            outlineColor.a = (unsigned char)(255 * alpha);
            DrawText(text, position.x + dx, position.y + dy, fontSize, outlineColor);
        }
    }
    
    // Main text (gradient effect)
    Color mainColor;
    if (comboDisplay.comboCount < 3) {
        mainColor = YELLOW;
    } else if (comboDisplay.comboCount < 5) {
        mainColor = ORANGE;
    } else {
        mainColor = RED;
    }
    mainColor.a = (unsigned char)(255 * alpha);
    
    DrawText(text, position.x, position.y, fontSize, mainColor);
}