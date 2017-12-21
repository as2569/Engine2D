#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

Building* point_to_building(int x, int y, Building* bld);

Entity* point_to_entity(int x, int y, Entity* ent);

Building* entity_within_building(Entity* ent, Building* bld);

//Vector2D vec_to_vec(Vector2D this_vec, Vector2D other_vec);

#endif
