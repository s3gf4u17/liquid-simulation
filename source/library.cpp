// struct Particle {
//     float px,py,vx,vy,ax,ay;
// };

// float smooth_gradient(float dist,float radius) {
//     float value=(radius>dist)?radius*radius-dist*dist:0.0;
//     return value*value*value;
// }

// void generate_particle(Particle *ps,int pi) {
//     ps[pi].px=0;//rand()%200/100.0-1.0;
//     ps[pi].py=0;//rand()%200/100.0-1.0;
//     ps[pi].vx=0;ps[pi].vy=0;
//     ps[pi].ax=0;ps[pi].ay=0;
// }

// void calculate_forces(Particle *ps,int pi,int pc) {
//     for (int i=0;i<pc;i++) {
//         if (i==pi) continue; // particle cant interact with itself
//         float distx=ps[i].px-ps[pi].px;
//         if (distx>SMOOTH) continue; // not affecting
//         float disty=ps[i].py-ps[pi].py;
//         if (disty>SMOOTH) continue; // not affecting
//         float distl=sqrt(distx*distx+disty*disty);
//         if (distl==0) continue;
//         if (distl>SMOOTH) continue; // not affecting
//         distx/=distl;disty/=distl; // get unit direction
//         float force=smooth_gradient(distl,SMOOTH); // get force
//         ps[pi].ax-=distx*force/MASS*PRESSURE;
//         ps[pi].ay-=disty*force/MASS*PRESSURE;
//         ps[i].ax+=distx*force/MASS*PRESSURE; // reaction opposite force
//         ps[i].ay+=disty*force/MASS*PRESSURE; // reaction opposite force
//     }
// }

// void calculate_movement(Particle *ps,int pi,float dt) {
//     ps[pi].px+=ps[pi].vx*dt+0.5*ps[pi].ax*dt*dt;
//     ps[pi].py+=ps[pi].vy*dt+0.5*ps[pi].ay*dt*dt-GRAVITY*0.5*dt*dt;
//     ps[pi].vx+=ps[pi].ax*dt;
//     ps[pi].vy+=ps[pi].ay*dt-GRAVITY*dt;
//     if (abs(ps[pi].px)>1.0) {
//         ps[pi].px=((ps[pi].px>0)?1:((ps[pi].px<0)?-1:0));
//         ps[pi].vx*=-0.8;
//     }
//     if (abs(ps[pi].py)>1.0) {
//         ps[pi].py=((ps[pi].py>0)?1:((ps[pi].py<0)?-1:0));
//         ps[pi].vy*=-0.8;
//     }
// }

// unsigned int generate_vbo(Particle *ps,int pc) {
//     float datastream[PARTICLES*3];
//     for (int pi=0;pi<pc;pi++) {
//         datastream[2*pi]=ps[pi].px;
//         datastream[2*pi+1]=ps[pi].py;
//     }
//     unsigned int VBO,VAO;
//     glGenVertexArrays(1,&VAO);
//     glGenBuffers(1,&VBO);
//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER,VBO);
//     glBufferData(GL_ARRAY_BUFFER,2*sizeof(float)*PARTICLES,datastream,GL_DYNAMIC_DRAW);
//     glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
//     glEnableVertexAttribArray(0);
//     return VBO;
// }

// void update_vbo(Particle *ps,int pc) {
//     float datastream[PARTICLES*3];
//     for (int pi=0;pi<pc;pi++) {
//         datastream[2*pi]=ps[pi].px;
//         datastream[2*pi+1]=ps[pi].py;
//     }
//     glBufferSubData(GL_ARRAY_BUFFER,0,2*sizeof(float)*PARTICLES,datastream);
// }