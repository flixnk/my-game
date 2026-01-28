#ifndef LEVELS_H
#define LEVELS_H

#include "block.h"
#include <raylib.h>
#include <vector>
#include "enemy.h"

struct RenderObject {
    Vector2 position;
    BlockType type; 
};

class Level {
    public:
        virtual ~Level() {}
        virtual void initLevel() = 0;
        virtual void draw(float alpha, Camera2D camera) = 0;

        void updateEnemies();
        bool checkCollisionWithPlayer(Rectangle playerHitbox);

        std::vector<std::vector<Block>>& getMap() { return map; }

    protected:
        Vector2 levelSize;
        Texture2D backgroundSprite;
        std::vector<std::vector<Block>> map;
        std::vector<Enemy> enemies;

        std::vector<RenderObject> renderList;
};

class Level1 : public Level {
    public:
    Level1();
    ~Level1();
    void initLevel() override;
    void draw(float alpha, Camera2D camera) override;

    private:
    Texture2D stoneBlockSprite;
    Texture2D flagSprite;
    Texture2D coinEnemySprite;
};

#endif