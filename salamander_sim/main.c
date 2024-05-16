#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
Vector2 gravity = {0, 10};
const int speed_const = 150;
float friction = 0.03;
const int scrnWidth = 1280;
const int scrnHeight = 720;

const int particle_amount = 50;

const float k = 10;
const float restLength = 10;

typedef struct
{
    //vellocity
    Vector2 vel;
    
    //position
    Vector2 pos;

    //limb position
    Vector2 front_right_leg_pos;
    Vector2 front_left_leg_pos;

    Vector2 hind_right_leg_pos;
    Vector2 hind_left_leg_pos;
    //acceleration
    Vector2 acc;

    //properties
    float mass;
    float radius;
    float leg_length;
    float push_torque;
    Color color;

}particles;

void particle_motion(particles particle[]);
void collison_detection(particles particle[]);
void spring_force(particles particle[]);
void draw_string(particles particle[]);
void draw_particle(particles particle[]);

int main()
{
    SetRandomSeed(1);
    
    particles particle[particle_amount];

    for(int i = 0; i < particle_amount; i++) {
        particle[i].mass = 3;
        particle[i].radius = 20;
        particle[i].pos = (Vector2){scrnWidth / 2, scrnHeight / 2};//GetRandomValue(particle[i].radius + particle[i].radius, scrnWidth - particle[i].radius);
        //GetRandomValue(particle[i].radius + particle[i].radius, scrnHeight - particle[i].radius);
        particle[i].acc.x = 0;
        particle[i].acc.y = 0;
        particle[i].vel.x = 0;
        particle[i].vel.y = 0;
        particle[i].color = BLACK;//(Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
    }
    particle[0].radius = 22;
    particle[4].radius = 16;
    particle[8].radius = 25;
    particle[12].radius = 29;
    particle[16].radius = 27;
    particle[20].radius = 22;
    particle[24].radius = 17;
    particle[28].radius = 15;
    particle[32].radius = 13;
    particle[36].radius = 11;
    particle[40].radius = 8;
    particle[44].radius = 6;
    particle[48].radius = 3;
    
    particle[0].front_left_leg_pos = (Vector2){scrnWidth/2, scrnHeight/2};
    particle[0].front_right_leg_pos = (Vector2){scrnWidth/2, scrnHeight/2};
    particle[0].hind_left_leg_pos = (Vector2){scrnWidth/2, scrnHeight/2};
    particle[0].hind_right_leg_pos = (Vector2){scrnWidth/2, scrnHeight/2};
    particle[0].leg_length = 75;
    particle[0].push_torque = 0;

    InitWindow(scrnWidth, scrnHeight, "Game");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if(IsKeyDown(KEY_W))
        {
            particle[0].vel.y -= speed_const;
        }
        if(IsKeyDown(KEY_S))
        {
            particle[0].vel.y += speed_const;
        }
        if(IsKeyDown(KEY_D))
        {
            particle[0].vel.x += speed_const;
        }
        if(IsKeyDown(KEY_A))
        {
            particle[0].vel.x -= speed_const;
        }
        //particle[0].pos = GetMousePosition();
        collison_detection(particle);
        particle_motion(particle);
        spring_force(particle);
        BeginDrawing();
            ClearBackground(WHITE);
            DrawFPS(10, 10);
            draw_particle(particle);
            draw_string(particle);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void draw_particle(particles particle[])
{
    //body
    for (int i = 0; i < particle_amount;)
    {
        DrawCircleLinesV(particle[i].pos, particle[i].radius, particle[i].color);
        i+= 4;
    }
    
    //front limbs
    float leg_magnitude = 50;
    Vector2 front_direction = Vector2Subtract(particle[4].pos, particle[8].pos);
    float front_direction_magnitude = Vector2Length(front_direction);
    Vector2 front_normalized_direction = Vector2Divide(front_direction, (Vector2){front_direction_magnitude, front_direction_magnitude});

    Vector2 front_leg = Vector2Scale(front_normalized_direction, leg_magnitude);
    Vector2 front_leg_right = Vector2Add((Vector2){-front_leg.y, front_leg.x}, front_leg);
    Vector2 front_leg_left = Vector2Add((Vector2){front_leg.y, -front_leg.x}, front_leg);

    
if (Vector2Distance(particle[8].pos, particle[0].front_left_leg_pos) > particle[0].leg_length) 
{
    particle[0].front_left_leg_pos = Vector2Add(particle[8].pos, front_leg_left);
    particle[8].pos = Vector2Add(particle[8].pos, Vector2Scale(front_normalized_direction, particle[0].push_torque)); 
}
else if(Vector2Distance(particle[8].pos, particle[0].front_right_leg_pos) > particle[0].leg_length)
{
    particle[0].front_right_leg_pos = Vector2Add(particle[8].pos, front_leg_right);
    particle[8].pos = Vector2Add(particle[8].pos, Vector2Scale(front_normalized_direction, particle[0].push_torque)); 

}

DrawLineV(particle[8].pos, particle[0].front_right_leg_pos, RED);
DrawLineV(particle[8].pos, particle[0].front_left_leg_pos, RED);
DrawCircleLinesV(particle[0].front_right_leg_pos, 15, RED);
DrawCircleLinesV(particle[0].front_left_leg_pos, 15, RED);

    //hind limbs
    Vector2 hind_direction = Vector2Subtract(particle[16].pos, particle[20].pos);
    float hind_direction_magnitude = Vector2Length(hind_direction);
    Vector2 hind_normalized_direction = Vector2Divide(hind_direction, (Vector2){hind_direction_magnitude, hind_direction_magnitude});

    Vector2 hind_leg = Vector2Scale(hind_normalized_direction, leg_magnitude);
    Vector2 hind_leg_right = Vector2Add((Vector2){-hind_leg.y, hind_leg.x}, hind_leg);
    Vector2 hind_leg_left = Vector2Add((Vector2){hind_leg.y, -hind_leg.x}, hind_leg);

    
if (Vector2Distance(particle[20].pos, particle[0].hind_left_leg_pos) > particle[0].leg_length) 
{
    particle[0].hind_left_leg_pos = Vector2Add(particle[20].pos, hind_leg_left);
}
else if(Vector2Distance(particle[20].pos, particle[0].hind_right_leg_pos) > particle[0].leg_length)
{
    particle[0].hind_right_leg_pos = Vector2Add(particle[20].pos, hind_leg_right);
}
    DrawLineV(particle[20].pos, particle[0].hind_right_leg_pos, RED);
    DrawLineV(particle[20].pos, particle[0].hind_left_leg_pos, RED);
    DrawCircleLinesV(particle[0].hind_right_leg_pos, 15, RED);
    DrawCircleLinesV(particle[0].hind_left_leg_pos, 15, RED);
}

void particle_motion(particles particle[])
{
    for (int i = 0; i < particle_amount; i++)
    {
        //velocity update
        float delta_time = GetFrameTime();
        particle[i].vel.x += particle[i].acc.x * delta_time;
        particle[i].vel.y += particle[i].acc.y * delta_time;

        //position update
        particle[i].pos.x += particle[i].vel.x * delta_time;
        particle[i].pos.y += particle[i].vel.y * delta_time;

        //gravity update
        //particle->vel = Vector2Add(particle[0].vel, Vector2Scale(gravity, delta_time));


        //friction
        particle[i].vel.x *= friction;
        particle[i].vel.y *= friction;
        if(IsKeyPressed(KEY_RIGHT))
        {
            friction += 0.005;
        }
        if(IsKeyPressed(KEY_LEFT))
        {
            friction -= 0.005;
        }
    }
}

void collison_detection(particles particle[])
{
    // for(int i = 0; i < particle_amount; i++)
    // {
    //     if(particle[i].pos.y + particle[i].radius > scrnHeight)
    //     {
    //         particle[i].vel.y = 0;
    //         particle[i].acc.y = 0;
    //         particle[i].pos.y = scrnHeight - particle->radius;
    //     }
    //     if(particle[i].pos.x + particle[i].radius > scrnWidth)
    //     {
    //         particle[i].vel.y = 0;
    //         particle[i].acc.y = 0;
    //     }
    // }
}

void spring_force(particles particle[])
{
    for (int i = 0; i < particle_amount; i++)
    {
        float distance = Vector2Distance(particle[i].pos, particle[i+1].pos);
        float x = distance - restLength;
        float force = -k * x;
        
        if(force < 200)
        {
            Vector2 subtract = Vector2Subtract(particle[i+1].pos, particle[i].pos);
            Vector2 force_vec = Vector2Scale(subtract, force);
            particle[i+1].vel.x += force_vec.x / particle[i].mass;
            particle[i+1].vel.y += force_vec.y / particle[i].mass;
        }else
        {
            force = 250;
            Vector2 subtract = Vector2Subtract(particle[i+1].pos, particle[i].pos);
            Vector2 force_vec = Vector2Scale(subtract, force);
            particle[i+1].vel.x += force_vec.x / particle[i].mass;
            particle[i+1].vel.y += force_vec.y / particle[i].mass;
        }
    }
}

void draw_string(particles particle[])
{
    for (int i = 0; i < particle_amount-1; i++)
    {
        DrawLineV(particle[i].pos, particle[i+1].pos, BLACK);
    }
}