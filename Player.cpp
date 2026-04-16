#include "Player.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "GunWeapon.h"


static std::string DirectionToAnimation(float angle)
{
    // Normalizamos el ángulo a 0–360
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;

    if (angle >= 337.5f || angle < 22.5f)
        return "run_front";

    if (angle < 67.5f)
        return "run_diag_front_right";

    if (angle < 112.5f)
        return "run_right";

    if (angle < 157.5f)
        return "run_diag_back_right";

    if (angle < 202.5f)
        return "run_back";

    if (angle < 247.5f)
        return "run_diag_back_left";

    if (angle < 292.5f)
        return "run_left";

    return "run_diag_front_left";
}

static std::string IdleFromRun(const std::string& runAnim)
{
    if (runAnim == "run_front") return "idle_front";
    if (runAnim == "run_back") return "idle_back";

    // laterales cualquiera vale
    if (runAnim == "run_left") return "idle_left";
    if (runAnim == "run_right") return "idle_right";

    // diagonales simplificamos
    if (runAnim == "run_diag_front_left") return "idle_left";
    if (runAnim == "run_diag_front_right") return "idle_right";
    if (runAnim == "run_diag_back_left") return "idle_left";
    if (runAnim == "run_diag_back_right") return "idle_right";

    return "idle_front";
}


Player::Player(Scene* myscene, std::string img_name, float side_size)
    : Sprite(myscene, img_name, Vector2(side_size, side_size))
{
    Game::camera.target = this;
    weapon = new GunWeapon();


    float frameW = 24;
    float frameH = 21;

    float startX = 3;
    float startY = 11;

    float sepX = 6;
    float sepY[4] = { 3, 3, 3, 3 };

    auto rowY = [&](int row)
        {
            float y = startY;
            for (int i = 0; i < row; i++)
                y += frameH + sepY[i];
            return y;
        };

    // --- RUN DIAG FRONT LEFT (fila 0, col 0–3)
    {
        Animation* a = new Animation();
        a->name = "run_diag_front_left";
        a->frameTime = 0.1f;
        float y = rowY(0);
        for (int i = 0; i < 4; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_diag_front_left"] = a;
    }

    // --- RUN FRONT (fila 0, col 4–7)
    {
        Animation* a = new Animation();
        a->name = "run_front";
        a->frameTime = 0.1f;
        float y = rowY(0);
        for (int i = 4; i < 8; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_front"] = a;
    }

    // --- RUN DIAG FRONT RIGHT (fila 0, col 8–11)
    {
        Animation* a = new Animation();
        a->name = "run_diag_front_right";
        a->frameTime = 0.1f;
        float y = rowY(0);
        for (int i = 8; i < 12; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_diag_front_right"] = a;
    }

    // --- RUN LEFT (fila 1, col 0–3)
    {
        Animation* a = new Animation();
        a->name = "run_left";
        a->frameTime = 0.1f;
        float y = rowY(1);
        for (int i = 0; i < 4; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_left"] = a;
    }

    // --- RUN RIGHT (fila 1, col 8–11)
    {
        Animation* a = new Animation();
        a->name = "run_right";
        a->frameTime = 0.1f;
        float y = rowY(1);
        for (int i = 8; i < 12; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_right"] = a;
    }

    // --- RUN DIAG BACK LEFT (fila 2, col 0–3)
    {
        Animation* a = new Animation();
        a->name = "run_diag_back_left";
        a->frameTime = 0.1f;
        float y = rowY(2);
        for (int i = 0; i < 4; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_diag_back_left"] = a;
    }

    // --- RUN BACK (fila 2, col 4–7)
    {
        Animation* a = new Animation();
        a->name = "run_back";
        a->frameTime = 0.1f;
        float y = rowY(2);
        for (int i = 4; i < 8; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_back"] = a;
    }

    // --- RUN DIAG BACK RIGHT (fila 2, col 8–11)
    {
        Animation* a = new Animation();
        a->name = "run_diag_back_right";
        a->frameTime = 0.1f;
        float y = rowY(2);
        for (int i = 8; i < 12; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_diag_back_right"] = a;
    }

    // THROW (fila 3, col 0–2)
    {
        Animation* a = new Animation();
        a->name = "throw";
        a->frameTime = 0.12f;
        float y = rowY(3);
        for (int i = 0; i < 3; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["throw"] = a;
    }

    // DEATH (fuera de la grid)
    {
        Animation* a = new Animation();
        a->name = "death";
        a->frameTime = 0.15f;
        a->loop = false;

        float y = rowY(3);

        a->frames.push_back({ 107, y, frameW, frameH });
        a->frames.push_back({ 137, y, frameW, frameH });

        animations["death"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_front";
        a->frameTime = 0.2f;
        a->frames.push_back({ 183, rowY(0), frameW, frameH });
        animations["idle_front"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_back";
        a->frameTime = 0.2f;
        a->frames.push_back({ 183, rowY(2), frameW, frameH });
        animations["idle_back"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_left";
        a->frameTime = 0.2f;
        a->frames.push_back({ startX + 0 * (frameW + sepX), rowY(1), frameW, frameH });
        animations["idle_left"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_right";
        a->frameTime = 0.2f;
        a->frames.push_back({ startX + 8 * (frameW + sepX), rowY(1), frameW, frameH });
        animations["idle_right"] = a;
    }

    // ANIMACIÓN POR DEFECTO
    SetAnimation("run_front");
}


Player::Player(Scene* myscene, std::string img_name, Transform transform, float side_size) : Sprite(myscene, img_name, transform, Vector2(side_size, side_size))
{
	Game::camera.target = this;
	weapon = new GunWeapon();

    float frameW = 24;
    float frameH = 21;

    float startX = 3;
    float startY = 11;

    float sepX = 6;
    float sepY[4] = { 3, 3, 3, 3 };

    auto rowY = [&](int row)
        {
            float y = startY;
            for (int i = 0; i < row; i++)
                y += frameH + sepY[i];
            return y;
        };


    // --- RUN DIAG FRONT LEFT (fila 0, col 0–3)
    {
        Animation* a = new Animation();
        a->name = "run_diag_front_left";
        a->frameTime = 0.1f;
        float y = rowY(0);
        for (int i = 0; i < 4; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_diag_front_left"] = a;
    }

    // --- RUN FRONT (fila 0, col 4–7)
    {
        Animation* a = new Animation();
        a->name = "run_front";
        a->frameTime = 0.1f;
        float y = rowY(0);
        for (int i = 4; i < 8; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_front"] = a;
    }

    // --- RUN DIAG FRONT RIGHT (fila 0, col 8–11)
    {
        Animation* a = new Animation();
        a->name = "run_diag_front_right";
        a->frameTime = 0.1f;
        float y = rowY(0);
        for (int i = 8; i < 12; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_diag_front_right"] = a;
    }

    // --- RUN LEFT (fila 1, col 0–3)
    {
        Animation* a = new Animation();
        a->name = "run_left";
        a->frameTime = 0.1f;
        float y = rowY(1);
        for (int i = 0; i < 4; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_left"] = a;
    }

    // --- RUN RIGHT (fila 1, col 8–11)
    {
        Animation* a = new Animation();
        a->name = "run_right";
        a->frameTime = 0.1f;
        float y = rowY(1);
        for (int i = 8; i < 12; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_right"] = a;
    }

    // --- RUN DIAG BACK LEFT (fila 2, col 0–3)
    {
        Animation* a = new Animation();
        a->name = "run_diag_back_left";
        a->frameTime = 0.1f;
        float y = rowY(2);
        for (int i = 0; i < 4; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_diag_back_left"] = a;
    }

    // --- RUN BACK (fila 2, col 4–7)
    {
        Animation* a = new Animation();
        a->name = "run_back";
        a->frameTime = 0.1f;
        float y = rowY(2);
        for (int i = 4; i < 8; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_back"] = a;
    }

    // --- RUN DIAG BACK RIGHT (fila 2, col 8–11)
    {
        Animation* a = new Animation();
        a->name = "run_diag_back_right";
        a->frameTime = 0.1f;
        float y = rowY(2);
        for (int i = 8; i < 12; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["run_diag_back_right"] = a;
    }

    // THROW (fila 3, col 0–2)

    {
        Animation* a = new Animation();
        a->name = "throw";
        a->frameTime = 0.12f;
        float y = rowY(3);
        for (int i = 0; i < 3; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });
        animations["throw"] = a;
    }

    // DEATH (fuera de la grid)

    {
        Animation* a = new Animation();
        a->name = "death";
        a->frameTime = 0.15f;
        a->loop = false;

        float y = rowY(3);

        a->frames.push_back({ 107, y, frameW, frameH });
        a->frames.push_back({ 137, y, frameW, frameH });

        animations["death"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_front";
        a->frameTime = 0.2f;
        a->frames.push_back({ 183, rowY(0), frameW, frameH });
        animations["idle_front"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_back";
        a->frameTime = 0.2f;
        a->frames.push_back({ 183, rowY(2), frameW, frameH });
        animations["idle_back"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_left";
        a->frameTime = 0.2f;
        a->frames.push_back({ startX + 0 * (frameW + sepX), rowY(1), frameW, frameH });
        animations["idle_left"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_right";
        a->frameTime = 0.2f;
        a->frames.push_back({ startX + 8 * (frameW + sepX), rowY(1), frameW, frameH });
        animations["idle_right"] = a;
    }

    // ANIMACIÓN POR DEFECTO
    SetAnimation("run_front");

}

void Player::Update(float deltaTime)
{
    Sprite::Update(deltaTime);
	UpdateMovement(deltaTime);
}

void Player::UpdateMovement(float deltaTime)
{
    if (isDead)
        return;

    Vector2 dir(0, 0);

    bool up = Game::keyDown[SDLK_W] || Game::keyDown[SDLK_UP];
    bool down = Game::keyDown[SDLK_S] || Game::keyDown[SDLK_DOWN];
    bool left = Game::keyDown[SDLK_A] || Game::keyDown[SDLK_LEFT];
    bool right = Game::keyDown[SDLK_D] || Game::keyDown[SDLK_RIGHT];

    if (up)    dir.y -= 1;
    if (down)  dir.y += 1;
    if (left)  dir.x -= 1;
    if (right) dir.x += 1;

    if (isThrowing)
    {
        Animation* a = currentAnimation;

        if (a->finished)
        {
            isThrowing = false;
            SetAnimation(IdleFromRun(lastMoveAnim));
        }
    }

    if (dir.x != 0 || dir.y != 0)
    {
        dir = dir.normalize();

        float speed = 300.f;
        transform.position += dir * speed * deltaTime;

        float angle = atan2(dir.y, dir.x) * 180.f / 3.14159f;
        angle += 90.f;
        transform.rotation = angle;

        std::string anim = DirectionToAnimation(angle);
        SetAnimation(anim);
        lastMoveAnim = anim;

    }
    else
    {
        SetAnimation(IdleFromRun(lastMoveAnim));
    }


    // Disparo
    if (Game::keyDown[SDLK_SPACE])
    {
        Game::keyDown[SDLK_SPACE] = false;

        Transform shot;
        shot.position = transform.position + Vector2(cos(transform.rotation * 0.01745f),
            sin(transform.rotation * 0.01745f)) * 50.f;

        shot.rotation = transform.rotation;

        weapon->Shoot(myScene, shot);
    }

    if (Game::keyDown[SDLK_G]/* && !grenadeActive*/) // o la tecla que quieras
    {
        Game::keyDown[SDLK_G] = false;

        Transform t;
        t.position = transform.position;
        t.rotation = transform.rotation;
        isThrowing = true;
        SetAnimation("throw");

        /*new Grenade(myScene, t, this);
        grenadeActive = true;*/
    }
}
//void Player::TakeDamage(int dmg)
//{
//    if (isDead) return;
//
//    health -= dmg;
//
//    if (health <= 0)
//    {
//        isDead = true;
//        SetAnimation("death");
//        Game::camera.target = NULL;
//    }
//}

