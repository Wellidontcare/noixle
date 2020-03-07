#include "calculation_parser.h"

CalcType CalculationParser::parse_calc_string(QString calc_string)
{
    if(calc_string.isNull()){
        throw std::logic_error("Error in " + std::string(__func__) + " missing input");
    }
    if(calc_string == "i"){
        return {ACTIVE_IMAGE, 0, 0};
    }
    if(calc_string.startsWith("s")){
        calc_string.remove(0, 1);
        return {SNAPSHOT, 0, calc_string.toInt()};
    }
    else{
        return {SCALAR, calc_string.toDouble(), 0};
    }
}
