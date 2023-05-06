#include <HelperFunctions.h>

int InvertDirection(Direction d)
{
	switch (d) {
	case (DirectionUp): return DirectionDown;
	case (DirectionDown): return DirectionUp;
	case (DirectionLeft): return DirectionRight;
	case (DirectionRight): return DirectionLeft;
	}
}
