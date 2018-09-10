#include <algorithm>

#include "TLorentzVector.h"

namespace math
{
    typedef TLorentzVector XYZTLorentzVector;
}

std::pair<math::XYZTLorentzVector,math::XYZTLorentzVector> NuMomentum(float leptonPx, float leptonPy, float leptonPz, float leptonPt, float leptonE, float metPx, float metPy );

