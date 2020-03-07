#ifndef CALCULATION_PARSER_H
#define CALCULATION_PARSER_H
#include <backend/image_processing/imageprocessingcollection.h>

enum CalcTypeEnum {
  ACTIVE_IMAGE,
  SCALAR,
  SNAPSHOT
};
struct CalcType {
  CalcTypeEnum type;
  double scalar;
  int snapshot_index;
};

class CalculationParser {
public:
  CalculationParser() = delete;
  static CalcType parse_calc_string(QString calc_string);

};

#endif // CALCULATION_PARSER_H
