// the implemented class (last)
#include "heatreductor.hpp"

void HeatReductor::field_reduction(Distributed2DField &data) const {
  double sum = 0;

  // sum the element of the local domain
  for (int yy = 0; yy < data.noghost_view().extent(DY); ++yy) {
    for (int xx = 0; xx < data.noghost_view().extent(DX); ++xx) {
      sum += data.full_view(yy, xx);
    }
  }

  double avg =
      sum / (data.noghost_view().extent(DY) * data.noghost_view().extent(DX));

  data.set_reducedValue(avg);
}
