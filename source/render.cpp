// const float gravity = 1;

class Vector2 {
public:
    float x,y;
    Vector2(float x,float y):x(x),y(y){}
    Vector2(float x):x(x),y(x){}
    Vector2():x(0),y(0){}
    Vector2 operator+(Vector2 right) {
        return Vector2(x+right.x,y+right.y);
    }
    Vector2 operator-(Vector2 right) {
        return Vector2(x-right.x,y-right.y);
    }
    Vector2 operator*(double multiplier) {
        return Vector2(x*multiplier,y*multiplier);
    }
    float magnitude() {
        return sqrt(x*x+y*y);
    }
};

float ExampleFunction(Vector2 pos) {
    return cos(pos.y*15-3+sin(pos.x*15));
}

class Particle {
public:
    float mass=3;
    float property;
    Vector2 bbox;
    Vector2 acceleration,velocity,position;
    Particle(Vector2 position) : position(position) {}
    void update_position(float deltatime,std::vector<Particle> *particles) {
        bbox = Vector2(1-(float)particlesize/SCR_WIDTH,1-(float)particlesize/SCR_HEIGHT);
        position = position+velocity*deltatime+(acceleration+Vector2(0,-gravity)*0.1)*deltatime*deltatime*0.5;
        velocity = velocity+(acceleration+Vector2(0,-gravity)*0.1)*deltatime;
        if (abs(position.x)>bbox.x) {
            position.x = bbox.x*((position.x>0)?1:((position.x<0)?-1:0));
            velocity.x *= -(1-hitenergyloss);
        }
        if (abs(position.y)>bbox.y) {
            position.y = bbox.y*((position.y>0)?1:((position.y<0)?-1:0));
            velocity.y *= -(1-hitenergyloss);
        }
        for (int i=0;i<particles->size();i++) {
            if (abs((*particles)[i].position.x-position.x)>smoothradius) continue;
            if (abs((*particles)[i].position.y-position.y)>smoothradius) continue;
            float dst = ((*particles)[i].position-position).magnitude();
            if (dst>smoothradius) continue;
            float value = smoothradius*smoothradius*smoothradius-dst*dst*dst;
            float force = value*value*value*value;
            acceleration=acceleration+(position-(*particles)[i].position)*force*(1/mass);
            // (*particles)[i].acceleration=(*particles)[i].acceleration+((*particles)[i].position-position)*force*(1/mass);
        }
    }
};

class Liquid {
private:
    std::vector<Particle> particles;
    float *datastream;
public:
    int count=0;
    Liquid(int rows,int columns) {
        // float spacingx = particlesize/SCR_WIDTH*8.0f;
        // float spacingy = particlesize/SCR_HEIGHT*8.0f;
        for (int i = 0 ; i < rows*columns; i++) {
            float x = (rand()%200)/100.0-1.0;
            float y = (rand()%200)/100.0-1.0;
            Particle particle(Vector2(x,y));
            particle.property = ExampleFunction(particle.position);
            particles.push_back(particle);
            count++;
        }
    }
    unsigned int generateVBO() {
        datastream = (float*)malloc(3*sizeof(float)*particles.size());
        for (int i = 0 ; i < particles.size();i++) {
            datastream[3*i] = particles[i].position.x;
            datastream[3*i+1] = particles[i].position.y;
            datastream[3*i+2] = 0.0;
        }
        unsigned int VBO,VAO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,3*sizeof(float)*(particles.size()),datastream,GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        return VBO;
    }
    void updateVBO(float deltatime) {
        for (int i = 0 ; i < particles.size();i++) {
            particles[i].update_position(deltatime,&particles);
            glBufferSubData(GL_ARRAY_BUFFER,i*3*sizeof(float),2*sizeof(float),&particles[i].position);
        }
    }
    float smoothingKernel(float radius,float dst) {
        float volume = M_PI*pow(radius,8)/4;
        float value = (radius*radius-dst*dst)>0?(radius*radius-dst*dst):0.0;
        return value*value*value/volume;
    }
    float calculateDensity() {
        float density = 0;
        for (Particle particle : particles) {
            float dst = particle.position.magnitude();
            float influence = smoothingKernel(0.5,dst);
            density+=particle.mass*influence;
        }
        return density;
    }
};