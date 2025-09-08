#include <LoadBalancerHeuristic.h>

#include <stdio.h>
#include <threads.h>

static uint64_t getId();

typedef struct
{
	GENERATE_HEURISTIC_BODY();
	uint64_t id;
} CustomHeuristic;

DEFINE_HEURISTIC(CustomHeuristic);

DEFINE_HEURISTIC_INIT(CustomHeuristic)
{
	CustomHeuristic* self = (CustomHeuristic*)heuristic;

	self->id = getId();
}

DEFINE_HEURISTIC_FUNCTION(CustomHeuristic)
{
	CustomHeuristic* self = (CustomHeuristic*)heuristic;

	return self->id;
}

uint64_t getId()
{
	static uint64_t counter = 0;

	return counter++;
}
