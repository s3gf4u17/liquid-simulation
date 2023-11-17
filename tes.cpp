const float gravity;
const float collisionDamping;
const float particleSize;
const int numParticles;
const float particleSpacing;
const Vector2 boundsSize;

Vector2[] positions;
Vector2[] velocities;

float SmoothingKernel(float radius,float dst) {
    float volume = PI*pow(radius,8)/4;
    float value = max(0,radius*radius-dst*dst);
    return pow(value,3)/volume;
}

float CalculateDensity(Vector2 samplePoint) {
    float density = 0;
    const float mass = 1;
    foreach (Vector2 position in positions) {
        float dst = (position-samplePoint).magnitude();
        float influence = SmoothingKernel(smoothingRadius,dst);
        density += mass*influence;
    }
    return density;
}

void Update() {
    for (int i=0;i<positions.Length;i++) {
        velocities += Vector2.down * gravity * Time.deltaTime;
        positions += velocities[i] * Time.deltaTime;
        ResolveCollisions(ref positions[i],ref velocities[i]);
        DrawCircle(positions[i],1,Color.lightBlue);
    }
}

void ResolveCollisions(Vector2 position, Vector2 velocity) {
    Vector2 halfBoundsSize = boundsSize/2-Vector2.one*particleSize;
    if (abs(position.x)>halfBoundsSize.x) {
        position.x=halfBoundsSize.x*sign(position.x);
        velocity.x*=-1*collisionDamping;
    }
    if (abs(position.y)>halfBoundsSize.y) {
        position.y=halfBoundsSize.y*sign(position.y);
        velocity.y*=-1*collisionDamping;
    }
}

void Start() {
    positions = new Vector2[numParticles];
    velocities = new Vector2[numParticles];

    int particlesPerRow = (int)sqrt(numParticles);
    int particlesPerCol = (numParticles-1)/particlesPerRow+1;
    float spacing = particleSize*2+particleSpacing;

    for (int i=0;i<numParticles;i++) {
        float x = (i%particlesPerRow-particlesPerRow/2.0f+0.5f)*spacing;
        float y = (i/particlesPerRow-particlesPerCol/2.0f+0.5f)*spacing;
        positions[i] = new Vector2(x,y);
    }
}