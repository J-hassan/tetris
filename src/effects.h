#pragma once
#include <raylib.h>
#include <vector>
#include "position.h"
#include "colors.h"

// Particle structure
struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float lifetime;
    float maxLifetime;
    float size;
    bool active;
};

// Line clear animation structure
struct LineClearAnimation {
    int row;
    float progress;        // 0.0 to 1.0
    float flashProgress;   // For flash effect
    bool active;
};

// Screen shake structure
struct ScreenShake {
    float intensity;
    float duration;
    float timer;
};

// Combo display structure
struct ComboDisplay {
    int comboCount;
    Vector2 position;
    float lifetime;
    float scale;
    bool active;
};

// Block landing effect
struct LandingEffect {
    std::vector<Position> positions;
    float progress;
    Color color;
    bool active;
};

class Effects {
public:
    Effects();
    
    // Particle system
    void AddParticles(Vector2 position, Color color, int count);
    void AddBlockLandParticles(std::vector<Position> positions, Color blockColor);
    void AddLineClearExplosion(int row);
    void AddLevelUpEffect(Vector2 position);
    
    // Animations
    void StartLineClearAnimation(std::vector<int> rows);
    void StartLandingEffect(std::vector<Position> positions, Color color);
    void StartComboDisplay(int combo, Vector2 position);
    void StartScreenShake(float intensity, float duration);
    
    // Update & Draw
    void Update(float deltaTime);
    void Draw();
    void DrawLineClearAnimations();
    
    // Query
    bool HasActiveLineClearAnimation();
    Vector2 GetScreenShakeOffset();
    
    // Reset
    void Reset();
    
private:
    std::vector<Particle> particles;
    std::vector<LineClearAnimation> lineClearAnims;
    LandingEffect landingEffect;
    ScreenShake screenShake;
    ComboDisplay comboDisplay;
    
    // Helper functions
    void UpdateParticles(float deltaTime);
    void UpdateLineClearAnimations(float deltaTime);
    void UpdateLandingEffect(float deltaTime);
    void UpdateScreenShake(float deltaTime);
    void UpdateComboDisplay(float deltaTime);
    
    void DrawParticles();
    void DrawLandingEffect();
    void DrawComboDisplay();
    
    // Constants
    static const int MAX_PARTICLES = 500;
    static constexpr float LINE_CLEAR_DURATION = 0.5f;
    static constexpr float LANDING_DURATION = 0.3f;
    static constexpr float COMBO_DURATION = 2.0f;
};