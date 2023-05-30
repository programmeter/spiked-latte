#ifndef CONVERSIONS_HPP
#define CONVERSIONS_HPP

// Convert inches to pixels
int inToPx(float in, int dpi)
{
    return in * dpi;
}

#endif