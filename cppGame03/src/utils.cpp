#include "utils.h"

bool CheckCollisionLineRec(Vector2 startPos, Vector2 endPos, Rectangle rec) {
    Vector2 topLeft = { rec.x, rec.y };
    Vector2 topRight = { rec.x + rec.width, rec.y };
    Vector2 bottomLeft = { rec.x, rec.y + rec.height };
    Vector2 bottomRight = { rec.x + rec.width, rec.y + rec.height };

    // Prüfen, ob die Linie eine der 4 Kanten des Rechtecks schneidet
    if (CheckCollisionLines(startPos, endPos, topLeft, topRight, nullptr) ||
        CheckCollisionLines(startPos, endPos, topRight, bottomRight, nullptr) ||
        CheckCollisionLines(startPos, endPos, bottomRight, bottomLeft, nullptr) ||
        CheckCollisionLines(startPos, endPos, bottomLeft, topLeft, nullptr)) {
        return true;
    }

    // Falls ein Punkt komplett innerhalb des Rechtecks liegt
    return CheckCollisionPointRec(startPos, rec) || CheckCollisionPointRec(endPos, rec);
}