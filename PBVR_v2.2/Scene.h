#ifndef SCENE_H
#define SCENE_H

#include "./include/Object.h"

template <typename I, typename I3, typename R, typename R3>
class Scene : public Object
{
public:
    Scene() {reset();}
    ~Scene() {}

private:
    I i_np;

    I* i_id;
    I* i_type;

    I i_nScalar;
    I i_nVector;

    R ** r_scalar;
    R3** r3_vector;

private:
    void reset()
    {
        i_np = 0;
        i_nScalar = 0;
        i_nVector = 0;

        i_id = NULL;
        i_type = NULL;

        r_scalar = NULL;
        r3_vector = NULL;
    }

};

#endif // SCENE_H
