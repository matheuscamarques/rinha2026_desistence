/* Auto-generated Optimized C Code - Rinha 2026 */
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define N_TARGET 1
#define MAX_N_CLASS 1

#ifndef LIKELY
#ifdef __GNUC__
#define LIKELY(x)      __builtin_expect(!!(x), 1)
#define UNLIKELY(x)    __builtin_expect(!!(x), 0)
#else
#define LIKELY(x)      (x)
#define UNLIKELY(x)    (x)
#endif
#endif

/* Estrutura fundamental para modelos float64 */
union Entry {
  int32_t missing;
  double fvalue;
  int32_t qvalue;
};

void predict_unit0(union Entry* data, double* result);
void postprocess(double* result);
void predict(union Entry* data, int pred_margin, double* result);




static const int32_t num_class[] = {  1, };

int32_t get_num_target(void) {
  return N_TARGET;
}
void get_num_class(int32_t* out) {
  for (int i = 0; i < N_TARGET; ++i) {
    out[i] = num_class[i];
  }
}
int32_t get_num_feature(void) {
  return 19;
}
const char* get_threshold_type(void) {
  return "float64";
}
const char* get_leaf_output_type(void) {
  return "float64";
}

void predict(union Entry* data, int pred_margin, double* result) {
  unsigned int tmp;
  predict_unit0(data, result);
  
  // Apply base_scores
  result[0] += 0;
  
  // Apply postprocessor
  if (!pred_margin) { postprocess(result); }
}

void postprocess(double* result) {
  // sigmoid
  const double alpha = (double)1;
  for (size_t i = 0; i < N_TARGET * MAX_N_CLASS; ++i) {
    result[i] = (double)(1) / ((double)(1) + exp(-alpha * result[i]));
  }
}




void predict_unit0(union Entry* data, double* result) {
  unsigned int tmp;
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.7328995523838635;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += -0.6284645616089025;
          } else {
            result[0] += -0.6162466202993138;
          }
        } else {
          result[0] += -0.6566580587088;
        }
      } else {
        if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.785400003194809071) ) ) {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5132499933242798962) ) ) {
            result[0] += -0.6533475180114677;
          } else {
            result[0] += -0.6590900087859416;
          }
        } else {
          result[0] += -0.6505099564563211;
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.266450002789497431) ) ) {
        result[0] += -0.6556145574121054;
      } else {
        result[0] += -0.6533874384456527;
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03836814927207705;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.06400941793117003;
          } else {
            result[0] += 0.07390841462281356;
          }
        } else {
          result[0] += 0.03711708943615517;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.1750000044703483859) ) ) {
          result[0] += 0.04473251050894116;
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8499500155448914684) ) ) {
            if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5023500025272370495) ) ) {
              result[0] += 0.03840938986570352;
            } else {
              result[0] += 0.03276062770149609;
            }
          } else {
            result[0] += 0.04164750606504014;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.03645000047981739738) ) ) {
          result[0] += 0.03589651257064243;
        } else {
          if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.04474999941885472038) ) ) {
            result[0] += 0.040241929222142536;
          } else {
            if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.05104999989271164634) ) ) {
              result[0] += 0.03367736854330297;
            } else {
              result[0] += 0.037527257845144726;
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1392500028014183322) ) ) {
          result[0] += 0.03245109669497848;
        } else {
          result[0] += 0.03831726425635527;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.0378994036442049;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.060752357415273614;
          } else {
            result[0] += 0.07048966174612445;
          }
        } else {
          result[0] += 0.037604294198716734;
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1684500053524971286) ) ) {
          result[0] += 0.034344672516906344;
        } else {
          result[0] += 0.039314841601687744;
        }
      }
    } else {
      if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1054500006139278551) ) ) {
        result[0] += 0.029366771117759462;
      } else {
        if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.339049994945526345) ) ) {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03714999929070473411) ) ) {
            result[0] += 0.03138311858314289;
          } else {
            result[0] += 0.03565927942146556;
          }
        } else {
          result[0] += 0.036871570717243486;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.037453712648077175;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.0585163205676265;
          } else {
            result[0] += 0.06745535860870068;
          }
        } else {
          result[0] += 0.03418117596845059;
        }
      } else {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1544499993324280063) ) ) {
          result[0] += 0.039095606070386285;
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2499499991536140719) ) ) {
            result[0] += 0.03057906150107055;
          } else {
            result[0] += 0.036035090226594395;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2777500003576279242) ) ) {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03714999929070473411) ) ) {
          result[0] += 0.03024829216062539;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4504000097513199408) ) ) {
            result[0] += 0.033823991566295716;
          } else {
            result[0] += 0.03482157148480947;
          }
        }
      } else {
        if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.556419610977173296) ) ) {
          result[0] += 0.03612462030348735;
        } else {
          result[0] += 0.042417300365789785;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.037029548222209586;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6700499951839448132) ) ) {
          result[0] += 0.03048384469451709;
        } else {
          result[0] += 0.06473877471845645;
        }
      } else {
        if ( LIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.4583500027656555731) ) ) {
          result[0] += 0.03283918841592163;
        } else {
          result[0] += 0.03729354123834461;
        }
      }
    } else {
      if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04924999922513962486) ) ) {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2437499985098839084) ) ) {
          if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.7750000059604645886) ) ) {
            result[0] += 0.03016096980314614;
          } else {
            result[0] += 0.03684870676128987;
          }
        } else {
          result[0] += 0.020768651681206986;
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.06284999847412110763) ) ) {
          result[0] += 0.035714926267364486;
        } else {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
            result[0] += 0.032660873989516295;
          } else {
            result[0] += 0.0337974368196175;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03662551654894511;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.0550842671492837;
          } else {
            result[0] += 0.06231087122197084;
          }
        } else {
          result[0] += 0.031244153832418207;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.03681986529618183;
        } else {
          result[0] += 0.0316894099469585;
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.266450002789497431) ) ) {
        result[0] += 0.03205403031504454;
      } else {
        result[0] += 0.03373584175212757;
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.036240343769976376;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6658999919891358532) ) ) {
          result[0] += 0.028558695578310478;
        } else {
          result[0] += 0.060105011618822236;
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2993499934673309881) ) ) {
          result[0] += 0.026387045211260996;
        } else {
          result[0] += 0.032597147880250796;
        }
      }
    } else {
      if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
        if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.4396259933710098822) ) ) {
          result[0] += 0.03273097180562075;
        } else {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.293586850166321245) ) ) {
            result[0] += 0.030945150428433814;
          } else {
            result[0] += 0.034139087378101535;
          }
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04955000057816506126) ) ) {
          result[0] += 0.025092333621182905;
        } else {
          result[0] += 0.0302291340308257;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.035872858210431784;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.04973336390704072;
          } else {
            result[0] += 0.05811730081029126;
          }
        } else {
          result[0] += 0.03026897949132602;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.0352785487009595;
        } else {
          result[0] += 0.03043208509480721;
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
        result[0] += 0.029645714256213172;
      } else {
        result[0] += 0.03069117136200002;
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03552198140585;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.048365984021039775;
          } else {
            result[0] += 0.056299161347843095;
          }
        } else {
          result[0] += 0.02722744363421564;
        }
      } else {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02564999926835298885) ) ) {
          if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01274999976158142263) ) ) {
            result[0] += 0.03386259482097396;
          } else {
            result[0] += 0.027035961854102366;
          }
        } else {
          result[0] += 0.032927308272100574;
        }
      }
    } else {
      if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
        result[0] += 0.029605664910403492;
      } else {
        result[0] += 0.028732568220568355;
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03518671851875361;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.04808687836971997;
          } else {
            result[0] += 0.05463556126661577;
          }
        } else {
          result[0] += 0.027087389588597764;
        }
      } else {
        if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.4249999970197678167) ) ) {
          result[0] += 0.030997394323554115;
        } else {
          result[0] += 0.02654897016861775;
        }
      }
    } else {
      if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.796943068504333718) ) ) {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03535000048577786186) ) ) {
          result[0] += 0.023752897490491066;
        } else {
          if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
            result[0] += 0.028385477696256586;
          } else {
            result[0] += 0.0274794441730367;
          }
        }
      } else {
        result[0] += 0.02966266715787925;
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03486615948818508;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6581000089645386852) ) ) {
          result[0] += 0.02358838780129642;
        } else {
          result[0] += 0.05310276696348109;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.03192659359454739;
        } else {
          result[0] += 0.027094872491990763;
        }
      }
    } else {
      if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.557041406631469949) ) ) {
        result[0] += 0.026998220968594985;
      } else {
        result[0] += 0.028310840243044765;
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03455945096422386;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.04529270206372862;
          } else {
            result[0] += 0.05170234642291071;
          }
        } else {
          result[0] += 0.029168851467410307;
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2967500090599060614) ) ) {
          result[0] += 0.023366654151870937;
        } else {
          result[0] += 0.028790719281110565;
        }
      }
    } else {
      if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.557041406631469949) ) ) {
        if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
          if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
            result[0] += 0.02576375168901198;
          } else {
            result[0] += 0.027140500769094543;
          }
        } else {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02894999925047159542) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01815000083297491421) ) ) {
              result[0] += 0.02420191486463085;
            } else {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02634999994188547481) ) ) {
                result[0] += 0.027214561504740865;
              } else {
                result[0] += 0.0235378679010431;
              }
            }
          } else {
            result[0] += 0.020356503365403154;
          }
        }
      } else {
        if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01925000082701444973) ) ) {
            result[0] += 0.030044243001462106;
          } else {
            result[0] += 0.026464990270244893;
          }
        } else {
          result[0] += 0.0262381913344932;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03426581394623822;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6658999919891358532) ) ) {
          result[0] += 0.021754938129580747;
        } else {
          result[0] += 0.0503946490544448;
        }
      } else {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2437499985098839084) ) ) {
          result[0] += 0.027416931426409455;
        } else {
          result[0] += 0.021417525693692954;
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.05285000056028366783) ) ) {
          result[0] += 0.0258865557248143;
        } else {
          result[0] += 0.02444739385511735;
        }
      } else {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2940499931573868353) ) ) {
          if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2499499991536140719) ) ) {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.08219999819993974166) ) ) {
              result[0] += 0.025326616573213334;
            } else {
              result[0] += 0.02731191608129805;
            }
          } else {
            result[0] += 0.025660719310179162;
          }
        } else {
          result[0] += 0.02955705809000862;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03398451957287724;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
          result[0] += 0.01993605646395658;
        } else {
          result[0] += 0.04919309731738801;
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2931499928236008246) ) ) {
          result[0] += 0.019643259570502874;
        } else {
          if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
            result[0] += 0.030419492305158537;
          } else {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
              result[0] += 0.02175558382012529;
            } else {
              result[0] += 0.02619577581985812;
            }
          }
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.266450002789497431) ) ) {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
          result[0] += 0.025029633306788264;
        } else {
          if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.8250000178813935436) ) ) {
            result[0] += 0.02392698516815444;
          } else {
            result[0] += 0.02533781407109342;
          }
        }
      } else {
        result[0] += 0.02640821814274837;
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03371489305543073;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          result[0] += 0.04807846029094403;
        } else {
          result[0] += 0.02506293649533156;
        }
      } else {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1544499993324280063) ) ) {
          result[0] += 0.028133951229059668;
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2499499991536140719) ) ) {
            result[0] += 0.02098733229582163;
          } else {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.330050006508827265) ) ) {
              result[0] += 0.02846745013513478;
            } else {
              result[0] += 0.023023591121676106;
            }
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04924999922513962486) ) ) {
        if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2499499991536140719) ) ) {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.339049994945526345) ) ) {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1174499988555908342) ) ) {
              result[0] += 0.020935861023056865;
            } else {
              result[0] += 0.028762295844802034;
            }
          } else {
            result[0] += 0.017328020431877672;
          }
        } else {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.0195500003173947369) ) ) {
            result[0] += 0.013304903311410583;
          } else {
            result[0] += 0.02349199353865898;
          }
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1157499998807907243) ) ) {
          result[0] += 0.025227126522894435;
        } else {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02615000028163195003) ) ) {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01305000018328428442) ) ) {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01245000027120113546) ) ) {
                result[0] += 0.02368603842024663;
              } else {
                result[0] += 0.020702531278046124;
              }
            } else {
              result[0] += 0.02467150051585244;
            }
          } else {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02634999994188547481) ) ) {
              result[0] += 0.017463644578726546;
            } else {
              result[0] += 0.023148200146376217;
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03345630893352575;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6581000089645386852) ) ) {
          result[0] += 0.019821916603000193;
        } else {
          result[0] += 0.04703505383315995;
        }
      } else {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2378500029444694797) ) ) {
          result[0] += 0.025143167346984366;
        } else {
          result[0] += 0.020579758965375703;
        }
      }
    } else {
      if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03714999929070473411) ) ) {
        result[0] += 0.019911866113574685;
      } else {
        if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.796943068504333718) ) ) {
          if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.2365819886326790134) ) ) {
            result[0] += 0.025361530266557797;
          } else {
            if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
              if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
                result[0] += 0.023035373532201982;
              } else {
                result[0] += 0.02413541112545142;
              }
            } else {
              result[0] += 0.02261326420344248;
            }
          }
        } else {
          result[0] += 0.02466587090867415;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.0332081831319835;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
          result[0] += 0.02115432449223743;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.03984722419431374;
          } else {
            result[0] += 0.04606590976352325;
          }
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2931499928236008246) ) ) {
          result[0] += 0.01989237669911654;
        } else {
          if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
            result[0] += 0.028458088045065326;
          } else {
            result[0] += 0.023342397132489687;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.266450002789497431) ) ) {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03285000100731850364) ) ) {
          result[0] += 0.016391154182677583;
        } else {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2466500028967857638) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1392500028014183322) ) ) {
              result[0] += 0.02041590229410865;
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1587999984622001926) ) ) {
                result[0] += 0.026150491541167015;
              } else {
                result[0] += 0.022982246414595064;
              }
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6796000003814698376) ) ) {
              result[0] += 0.023564482163425746;
            } else {
              result[0] += 0.02126179798790721;
            }
          }
        }
      } else {
        result[0] += 0.024324877256859653;
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03296997176042916;
  } else {
    if ( LIKELY( !(data[16].missing != -1) || (data[16].fvalue <= (double)0.07391583546996118026) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.4583500027656555731) ) ) {
          result[0] += 0.018123838251626195;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.698049992322921864) ) ) {
            result[0] += 0.03592027812868424;
          } else {
            result[0] += 0.04516162148409484;
          }
        }
      } else {
        result[0] += 0.02324052848756792;
      }
    } else {
      if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2466500028967857638) ) ) {
        if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.339049994945526345) ) ) {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03714999929070473411) ) ) {
            result[0] += 0.017645775991636962;
          } else {
            if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.2365819886326790134) ) ) {
              result[0] += 0.024443437234492604;
            } else {
              result[0] += 0.021916174124651435;
            }
          }
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.08219999819993974166) ) ) {
            result[0] += 0.021159035625972576;
          } else {
            if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.5250000059604645886) ) ) {
              result[0] += 0.02389786672059523;
            } else {
              result[0] += 0.021287841181485943;
            }
          }
        }
      } else {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02544999960809946407) ) ) {
          if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.2365819886326790134) ) ) {
            result[0] += 0.016314123749362922;
          } else {
            result[0] += 0.021851134950865037;
          }
        } else {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02845000009983778347) ) ) {
            if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2041499987244606296) ) ) {
              result[0] += 0.01801021763190311;
            } else {
              result[0] += 0.011297766062984644;
            }
          } else {
            result[0] += 0.02204281958537064;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03274116706831226;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.03870004156224836;
          } else {
            result[0] += 0.0443126539386228;
          }
        } else {
          result[0] += 0.020222445970437066;
        }
      } else {
        result[0] += 0.022475888241199084;
      }
    } else {
      if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.339049994945526345) ) ) {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
          if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2793499976396561224) ) ) {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01214999984949827368) ) ) {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.009349999949336053676) ) ) {
                result[0] += 0.022375583089112194;
              } else {
                result[0] += 0.018938920952432714;
              }
            } else {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02634999994188547481) ) ) {
                result[0] += 0.023867806294341334;
              } else {
                result[0] += 0.0208667375096208;
              }
            }
          } else {
            if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
              result[0] += 0.019170502110223153;
            } else {
              result[0] += 0.022162117259533543;
            }
          }
        } else {
          if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01274999976158142263) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01185000035911798651) ) ) {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.007050000131130219373) ) ) {
                result[0] += 0.018295345870402747;
              } else {
                result[0] += 0.020595030788110123;
              }
            } else {
              if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2352500036358833591) ) ) {
                result[0] += 0.01192604553948472;
              } else {
                result[0] += 0.022277454345754476;
              }
            }
          } else {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.04395000077784062126) ) ) {
              result[0] += 0.0163972476274101;
            } else {
              if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3739500045776367743) ) ) {
                result[0] += 0.02161038850506348;
              } else {
                result[0] += 0.018438876887383906;
              }
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04924999922513962486) ) ) {
          result[0] += 0.019007303367396738;
        } else {
          if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005649999948218465719) ) ) {
            result[0] += 0.026493083795852256;
          } else {
            result[0] += 0.022509340283020835;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03252129211288779;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6700499951839448132) ) ) {
          result[0] += 0.01744667760218645;
        } else {
          result[0] += 0.043509571865543065;
        }
      } else {
        result[0] += 0.02191931141123182;
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07954999804496766524) ) ) {
          if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.2365819886326790134) ) ) {
            result[0] += 0.02242590580638417;
          } else {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3372499942779541571) ) ) {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03714999929070473411) ) ) {
                result[0] += 0.01634575164543971;
              } else {
                result[0] += 0.02049469666955561;
              }
            } else {
              result[0] += 0.018777396226205514;
            }
          }
        } else {
          result[0] += 0.025030673450318393;
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.06284999847412110763) ) ) {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1174499988555908342) ) ) {
            result[0] += 0.013869088764226552;
          } else {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2499499991536140719) ) ) {
              result[0] += 0.021616962956192136;
            } else {
              result[0] += 0.01783231346165525;
            }
          }
        } else {
          result[0] += 0.021675165356905376;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.0323099059479675;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.03358110087643351;
          } else {
            result[0] += 0.04276622525933684;
          }
        } else {
          result[0] += 0.019796631578297468;
        }
      } else {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.06909999996423722701) ) ) {
          result[0] += 0.021467117331699277;
        } else {
          result[0] += 0.016895396332592103;
        }
      }
    } else {
      if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.006649999879300595197) ) ) {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2721499949693680365) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4175499975681305487) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2776999920606613714) ) ) {
              result[0] += 0.0188380551812421;
            } else {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005750000011175871763) ) ) {
                result[0] += 0.014907346517022154;
              } else {
                result[0] += 0.007768667841967955;
              }
            }
          } else {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.006349999923259020719) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6398000121116639294) ) ) {
                result[0] += 0.023573071663475782;
              } else {
                result[0] += 0.01903496312800668;
              }
            } else {
              result[0] += 0.013113386248003261;
            }
          }
        } else {
          result[0] += 0.024735972521902145;
        }
      } else {
        if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.5250000059604645886) ) ) {
          result[0] += 0.02074941141511751;
        } else {
          if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.1750000044703483859) ) ) {
            result[0] += 0.022370859628203472;
          } else {
            result[0] += 0.019130524343083474;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03210659086811917;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            result[0] += 0.035217512933132175;
          } else {
            result[0] += 0.042060478829483654;
          }
        } else {
          result[0] += 0.018506043430957783;
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2931499928236008246) ) ) {
          result[0] += 0.017484465616893395;
        } else {
          result[0] += 0.0219150745902799;
        }
      }
    } else {
      if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2466500028967857638) ) ) {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005950000137090683851) ) ) {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005750000011175871763) ) ) {
            result[0] += 0.018476870227142592;
          } else {
            result[0] += 0.011681399186271495;
          }
        } else {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04955000057816506126) ) ) {
            if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2318500056862831393) ) ) {
              result[0] += 0.018979870877130624;
            } else {
              result[0] += 0.01114290036728086;
            }
          } else {
            if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.0753500014543533464) ) ) {
              if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
                result[0] += 0.01975684966449417;
              } else {
                result[0] += 0.02071152077794226;
              }
            } else {
              result[0] += 0.022217751777309697;
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.006049999967217446241) ) ) {
          result[0] += 0.023225341276514568;
        } else {
          if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.009949999861419202632) ) ) {
            if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.082304060459137185) ) ) {
              result[0] += 0.01171462509881153;
            } else {
              result[0] += 0.01773174636956524;
            }
          } else {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02585000079125166286) ) ) {
              if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07324999943375588851) ) ) {
                result[0] += 0.019410031824094554;
              } else {
                result[0] += 0.023873034199645506;
              }
            } else {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02874999959021807064) ) ) {
                result[0] += 0.015770757012073252;
              } else {
                result[0] += 0.020676760386341315;
              }
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.031910962400413134;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6581000089645386852) ) ) {
          result[0] += 0.014906873113102384;
        } else {
          result[0] += 0.0413892291536695;
        }
      } else {
        if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.5250000059604645886) ) ) {
          result[0] += 0.019933123489974912;
        } else {
          result[0] += 0.015547517994205536;
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2606499940156937201) ) ) {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01515000034123659307) ) ) {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.06284999847412110763) ) ) {
            result[0] += 0.02100716884988533;
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4301500022411346991) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2931499928236008246) ) ) {
                result[0] += 0.018867057858639322;
              } else {
                result[0] += 0.015503186075578879;
              }
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4944000095129013617) ) ) {
                result[0] += 0.021113142411045095;
              } else {
                result[0] += 0.018476206563046018;
              }
            }
          }
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8777499794960023083) ) ) {
            if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8368499875068665661) ) ) {
              result[0] += 0.019776329327222078;
            } else {
              result[0] += 0.02429831908198781;
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.9093500077724457897) ) ) {
              result[0] += 0.01275396223406911;
            } else {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2396500036120415011) ) ) {
                result[0] += 0.019938993226914035;
              } else {
                result[0] += 0.017318337120713147;
              }
            }
          }
        }
      } else {
        if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2744500041007996161) ) ) {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.08874999731779099899) ) ) {
            result[0] += 0.018494099954772313;
          } else {
            result[0] += 0.021902298749019902;
          }
        } else {
          result[0] += 0.017220415802889823;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03172264745846758;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6581000089645386852) ) ) {
          result[0] += 0.015789683472489675;
        } else {
          result[0] += 0.04076102671180033;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
          if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
            result[0] += 0.020180408314310172;
          } else {
            result[0] += 0.015436957047295979;
          }
        } else {
          result[0] += 0.02054044660423629;
        }
      }
    } else {
      if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03535000048577786186) ) ) {
        result[0] += 0.015211691978383542;
      } else {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2777500003576279242) ) ) {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.382422208786011186) ) ) {
            if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2466500028967857638) ) ) {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2396500036120415011) ) ) {
                result[0] += 0.01892845296934132;
              } else {
                result[0] += 0.021752290673968725;
              }
            } else {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2670499980449677069) ) ) {
                result[0] += 0.01676023031701793;
              } else {
                result[0] += 0.018779196440556225;
              }
            }
          } else {
            result[0] += 0.013241149212317399;
          }
        } else {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.470308303833008257) ) ) {
            result[0] += 0.019432161535013436;
          } else {
            result[0] += 0.024210599841630678;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03154130732169478;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01035000011324882681) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6443499922752381481) ) ) {
          result[0] += 0.015106010727920016;
        } else {
          result[0] += 0.04016788170699691;
        }
      } else {
        result[0] += 0.019525849644275343;
      }
    } else {
      if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03934999927878380516) ) ) {
        result[0] += 0.015438066606809511;
      } else {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.266450002789497431) ) ) {
          if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
            result[0] += 0.01868927644617714;
          } else {
            result[0] += 0.017993053596019378;
          }
        } else {
          result[0] += 0.01969705527669387;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03136661505231445;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
          result[0] += 0.015863278992195872;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.03334886547754797;
          } else {
            result[0] += 0.03960935343057848;
          }
        }
      } else {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.0677499994635582109) ) ) {
          if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.009650000371038915462) ) ) {
            result[0] += 0.01419201949660099;
          } else {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01395000005140900785) ) ) {
              result[0] += 0.023066267685969703;
            } else {
              result[0] += 0.018377701040628254;
            }
          }
        } else {
          result[0] += 0.012453129511187652;
        }
      }
    } else {
      if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02715000044554472317) ) ) {
          result[0] += 0.01859815232748871;
        } else {
          if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.05525000020861626365) ) ) {
            if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.005949999904260040197) ) ) {
              result[0] += 0.01898483610771933;
            } else {
              result[0] += 0.0135772388379125;
            }
          } else {
            result[0] += 0.020622885084971263;
          }
        }
      } else {
        if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.470308303833008257) ) ) {
          if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07954999804496766524) ) ) {
            if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.05285000056028366783) ) ) {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.008750000037252904719) ) ) {
                result[0] += 0.015121128794391965;
              } else {
                result[0] += 0.018503990401444632;
              }
            } else {
              if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
                result[0] += 0.015938320571011605;
              } else {
                result[0] += 0.017459150186766265;
              }
            }
          } else {
            if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2100499942898750583) ) ) {
              result[0] += 0.02233600512948351;
            } else {
              result[0] += 0.016626958900878005;
            }
          }
        } else {
          result[0] += 0.022672360719692718;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.031198270600123;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.031052673367307895;
          } else {
            result[0] += 0.039080768706670474;
          }
        } else {
          result[0] += 0.014454567561437329;
        }
      } else {
        result[0] += 0.018345566827083316;
      }
    } else {
      if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.470308303833008257) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04175000078976155021) ) ) {
          result[0] += 0.012079606660536308;
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03745000064373017051) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.0221499996259808575) ) ) {
              if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.6739499866962433972) ) ) {
                result[0] += 0.020148482452813553;
              } else {
                result[0] += 0.012543765864876176;
              }
            } else {
              result[0] += 0.008541021758379019;
            }
          } else {
            result[0] += 0.01736544690894723;
          }
        }
      } else {
        if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2833500057458878119) ) ) {
          result[0] += 0.016551939073962248;
        } else {
          result[0] += 0.023550316507640286;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.031035990320311384;
  } else {
    if ( LIKELY( !(data[16].missing != -1) || (data[16].fvalue <= (double)0.07391583546996118026) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6658999919891358532) ) ) {
          result[0] += 0.01599971175535549;
        } else {
          result[0] += 0.038569458791308256;
        }
      } else {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02434999961405992855) ) ) {
          if ( UNLIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.3750000000000000555) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5703499913215638317) ) ) {
              result[0] += 0.01549918509185353;
            } else {
              result[0] += 0.010137103750628968;
            }
          } else {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
              result[0] += 0.013056423234940481;
            } else {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01894999947398901333) ) ) {
                result[0] += 0.020796927796019955;
              } else {
                result[0] += 0.014382939221671656;
              }
            }
          }
        } else {
          result[0] += 0.02094980504964181;
        }
      }
    } else {
      if ( UNLIKELY( !(data[16].missing != -1) || (data[16].fvalue <= (double)0.1417930796742439548) ) ) {
        if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.130882143974304643) ) ) {
          result[0] += 0.01715262548684701;
        } else {
          result[0] += 0.020431918509491272;
        }
      } else {
        if ( UNLIKELY( !(data[16].missing != -1) || (data[16].fvalue <= (double)0.1643003895878792087) ) ) {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2811499983072281439) ) ) {
            if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
              result[0] += 0.01378899815039048;
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4272000044584274847) ) ) {
                result[0] += 0.01497547426509648;
              } else {
                result[0] += 0.017379247173691253;
              }
            }
          } else {
            result[0] += 0.010970290004983263;
          }
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2931499928236008246) ) ) {
            if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1157499998807907243) ) ) {
              if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.2250000014901161471) ) ) {
                result[0] += 0.019852932895279618;
              } else {
                result[0] += 0.015234796738568685;
              }
            } else {
              result[0] += 0.00872185195819002;
            }
          } else {
            if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.5250000059604645886) ) ) {
              if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.339049994945526345) ) ) {
                result[0] += 0.01686932379311393;
              } else {
                result[0] += 0.018160917633994304;
              }
            } else {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2130500003695488254) ) ) {
                result[0] += 0.016929694339823595;
              } else {
                result[0] += 0.012474785161609147;
              }
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.03087949321113004;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.029083048504806187;
          } else {
            result[0] += 0.03809719014832089;
          }
        } else {
          result[0] += 0.015273960986878282;
        }
      } else {
        if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2601500004529953558) ) ) {
          result[0] += 0.01786216263889383;
        } else {
          result[0] += 0.013142111164208334;
        }
      }
    } else {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02615000028163195003) ) ) {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005450000055134297285) ) ) {
          result[0] += 0.01946768202330398;
        } else {
          if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.5434499979019166149) ) ) {
            if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2886500060558319647) ) ) {
              if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.3695499897003174383) ) ) {
                result[0] += 0.016239821307577933;
              } else {
                result[0] += 0.017770659413183718;
              }
            } else {
              result[0] += 0.01276403659190972;
            }
          } else {
            if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2886500060558319647) ) ) {
              result[0] += 0.0162569200056354;
            } else {
              result[0] += 0.01946161253305694;
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.2365819886326790134) ) ) {
          result[0] += 0.020775317155691975;
        } else {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02825000043958425869) ) ) {
              if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.05285000056028366783) ) ) {
                result[0] += 0.014381263782904228;
              } else {
                result[0] += 0.01106073030703987;
              }
            } else {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02894999925047159542) ) ) {
                result[0] += 0.017564244159732085;
              } else {
                result[0] += 0.013503833170571902;
              }
            }
          } else {
            if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.4565500020980835516) ) ) {
              result[0] += 0.018661717550368495;
            } else {
              result[0] += 0.015818723346469547;
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.030728533047598598;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
          result[0] += 0.011205211986207508;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.03096977893804132;
          } else {
            result[0] += 0.03763693485400527;
          }
        }
      } else {
        if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2061500027775764743) ) ) {
          if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.05724999867379666069) ) ) {
            result[0] += 0.018512289812097363;
          } else {
            result[0] += 0.01360018242528936;
          }
        } else {
          if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2203500047326088229) ) ) {
            result[0] += 0.017238998901310993;
          } else {
            result[0] += 0.009810247436757548;
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1054500006139278551) ) ) {
        result[0] += 0.011193733399089302;
      } else {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03535000048577786186) ) ) {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02124999929219484676) ) ) {
            result[0] += 0.014542051830504301;
          } else {
            result[0] += 0.008594000299858835;
          }
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.04725000075995922783) ) ) {
            if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
              result[0] += 0.021190418277395505;
            } else {
              result[0] += 0.017081582460280572;
            }
          } else {
            if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
              if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.130882143974304643) ) ) {
                result[0] += 0.015378652642105611;
              } else {
                result[0] += 0.017758202039074186;
              }
            } else {
              if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
                result[0] += 0.016872976411146583;
              } else {
                result[0] += 0.015380718863922985;
              }
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.030582869395904065;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01095000002533197576) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.02857601719181699;
          } else {
            result[0] += 0.03720774298643425;
          }
        } else {
          result[0] += 0.015392290117968834;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.02033643005095961;
        } else {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2155499979853630343) ) ) {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
              result[0] += 0.009781246810599907;
            } else {
              if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
                result[0] += 0.009995830582788457;
              } else {
                result[0] += 0.01966903056802116;
              }
            }
          } else {
            if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.474999994039535578) ) ) {
              result[0] += 0.0060447415139656124;
            } else {
              result[0] += 0.011257135846740703;
            }
          }
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.266450002789497431) ) ) {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005850000074133277807) ) ) {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005649999948218465719) ) ) {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005249999929219485197) ) ) {
              result[0] += 0.011584362126780639;
            } else {
              result[0] += 0.015879012282861964;
            }
          } else {
            result[0] += 0.007706647342946458;
          }
        } else {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.08874999731779099899) ) ) {
            if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.413049995899200495) ) ) {
              result[0] += 0.01829913703107709;
            } else {
              if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.5000000149011613049) ) ) {
                result[0] += 0.012974093579238904;
              } else {
                result[0] += 0.016497164966794528;
              }
            }
          } else {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01454999996349215681) ) ) {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1684500053524971286) ) ) {
                result[0] += 0.01346342716672661;
              } else {
                result[0] += 0.01546450994102971;
              }
            } else {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02075000014156103481) ) ) {
                result[0] += 0.016570919986499406;
              } else {
                result[0] += 0.01500068468530464;
              }
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.009150000289082528895) ) ) {
          result[0] += 0.021095769369361233;
        } else {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.06284999847412110763) ) ) {
            result[0] += 0.01031532332309502;
          } else {
            if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.5250000059604645886) ) ) {
              result[0] += 0.017571839841873414;
            } else {
              result[0] += 0.012044100338218367;
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.030442269587139265;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01095000002533197576) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.029301450539831006;
          } else {
            result[0] += 0.036795648744373244;
          }
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1909499987959862033) ) ) {
            result[0] += 0.021879876329372012;
          } else {
            result[0] += 0.01118743045345911;
          }
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.01916706305882864;
        } else {
          if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01095000002533197576) ) ) {
            result[0] += 0.00988689218407907;
          } else {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
              result[0] += 0.010184730310615876;
            } else {
              if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2100499942898750583) ) ) {
                result[0] += 0.018303798856083645;
              } else {
                result[0] += 0.01238025219494116;
              }
            }
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.2365819886326790134) ) ) {
        result[0] += 0.017107059695202844;
      } else {
        if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
          if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07954999804496766524) ) ) {
            result[0] += 0.015271493815267947;
          } else {
            result[0] += 0.01750953455228085;
          }
        } else {
          result[0] += 0.014524184047828058;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.030306523786848154;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.027144375352589196;
          } else {
            result[0] += 0.03640255896497;
          }
        } else {
          result[0] += 0.012838098770795179;
        }
      } else {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1544499993324280063) ) ) {
          result[0] += 0.016973441232552948;
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2499499991536140719) ) ) {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2129499986767769137) ) ) {
              result[0] += 0.012474171247130473;
            } else {
              result[0] += 0.007969228592873717;
            }
          } else {
            result[0] += 0.015108365175615708;
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.5434499979019166149) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.06284999847412110763) ) ) {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04955000057816506126) ) ) {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04385000094771385887) ) ) {
              result[0] += 0.0175908605087619;
            } else {
              result[0] += 0.01139755118040233;
            }
          } else {
            if ( UNLIKELY( !(data[15].missing != -1) || (data[15].fvalue <= (double)0.6218318045139313854) ) ) {
              result[0] += 0.014736021855801435;
            } else {
              result[0] += 0.020854813678136738;
            }
          }
        } else {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1684500053524971286) ) ) {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1157499998807907243) ) ) {
              result[0] += 0.015584086900320442;
            } else {
              result[0] += 0.013597359227683492;
            }
          } else {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
              result[0] += 0.01789763800738373;
            } else {
              result[0] += 0.015375132177036076;
            }
          }
        }
      } else {
        if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3739500045776367743) ) ) {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2466500028967857638) ) ) {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.0402499996125698159) ) ) {
              result[0] += 0.010099998992395203;
            } else {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2269499972462654391) ) ) {
                result[0] += 0.01487307353975267;
              } else {
                result[0] += 0.017100935719569638;
              }
            }
          } else {
            if ( UNLIKELY( !(data[15].missing != -1) || (data[15].fvalue <= (double)0.5056466758251191296) ) ) {
              result[0] += 0.011056652228725372;
            } else {
              result[0] += 0.013816968441025099;
            }
          }
        } else {
          if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.760849982500076405) ) ) {
            result[0] += 0.010172868217986925;
          } else {
            result[0] += 0.013702079466573416;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.030175428834423715;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6700499951839448132) ) ) {
          result[0] += 0.012550449633550977;
        } else {
          result[0] += 0.03601871318049949;
        }
      } else {
        if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
          if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
            result[0] += 0.014282829934296035;
          } else {
            result[0] += 0.018833360862751895;
          }
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5057500004768372692) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3194500058889389593) ) ) {
                result[0] += 0.010470342629013495;
              } else {
                result[0] += 0.016064745752833454;
              }
            } else {
              result[0] += 0.006499751200313117;
            }
          } else {
            result[0] += 0.01616925071777828;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.796943068504333718) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04955000057816506126) ) ) {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2440500035881996432) ) ) {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3006500005722046454) ) ) {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1174499988555908342) ) ) {
                result[0] += 0.007863914879947222;
              } else {
                result[0] += 0.014195637059504902;
              }
            } else {
              result[0] += 0.006470911255132002;
            }
          } else {
            result[0] += 0.01636283280254914;
          }
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1587999984622001926) ) ) {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2574499994516373236) ) ) {
              result[0] += 0.014310106096888374;
            } else {
              result[0] += 0.020514069673871076;
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2993499934673309881) ) ) {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2776999920606613714) ) ) {
                result[0] += 0.013866392947495965;
              } else {
                result[0] += 0.010219570501855969;
              }
            } else {
              if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.5000000149011613049) ) ) {
                result[0] += 0.015115918203408822;
              } else {
                result[0] += 0.01421258697428038;
              }
            }
          }
        }
      } else {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2886500060558319647) ) ) {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7055499851703644909) ) ) {
            if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.8912999927997590222) ) ) {
              result[0] += 0.01657206021292126;
            } else {
              result[0] += 0.011940958195375011;
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8546000123023987927) ) ) {
              result[0] += 0.00909706463487495;
            } else {
              result[0] += 0.014250470111479884;
            }
          }
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8099499940872193493) ) ) {
            if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.460050001740455683) ) ) {
              result[0] += 0.018791562472539165;
            } else {
              result[0] += 0.012032770136093204;
            }
          } else {
            result[0] += 0.020827546959754507;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.030048783263099916;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3750000000000000555) ) ) {
          result[0] += 0.008849615727355977;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6889500021934510388) ) ) {
            result[0] += 0.021839055914816087;
          } else {
            result[0] += 0.03566842814746527;
          }
        }
      } else {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01395000005140900785) ) ) {
          result[0] += 0.018134534016610623;
        } else {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02564999926835298885) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02165000047534704555) ) ) {
              result[0] += 0.013268371426951253;
            } else {
              result[0] += 0.009254534777578287;
            }
          } else {
            result[0] += 0.015508248045855657;
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1157499998807907243) ) ) {
        if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.8043499886989594616) ) ) {
          if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.3260499984025955755) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2339999973773956576) ) ) {
              result[0] += 0.009151570073288283;
            } else {
              result[0] += 0.013841760317739728;
            }
          } else {
            if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1692000031471252719) ) ) {
              if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3666500002145767767) ) ) {
                result[0] += 0.015155062718113144;
              } else {
                result[0] += 0.011714614091303118;
              }
            } else {
              result[0] += 0.016746342259457658;
            }
          }
        } else {
          if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.5250000059604645886) ) ) {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1174499988555908342) ) ) {
              if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
                result[0] += 0.005995821791750105;
              } else {
                result[0] += 0.012537763928084984;
              }
            } else {
              if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.4249999970197678167) ) ) {
                result[0] += 0.014291608981332493;
              } else {
                result[0] += 0.009000225249527753;
              }
            }
          } else {
            result[0] += 0.016900938748392786;
          }
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1684500053524971286) ) ) {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
            result[0] += 0.012019154186748333;
          } else {
            if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.082304060459137185) ) ) {
              result[0] += 0.019154982009273465;
            } else {
              result[0] += 0.012853355098067908;
            }
          }
        } else {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2601500004529953558) ) ) {
            if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.210552096366882768) ) ) {
              result[0] += 0.014282755798192631;
            } else {
              result[0] += 0.016908712204705748;
            }
          } else {
            if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.5434499979019166149) ) ) {
              if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.3695499897003174383) ) ) {
                result[0] += 0.012285393337148927;
              } else {
                result[0] += 0.01575448887284087;
              }
            } else {
              if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.6580573916435242809) ) ) {
                result[0] += 0.008276815985835172;
              } else {
                result[0] += 0.013040492614785806;
              }
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.02992641459440139;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.0268699684767412;
          } else {
            result[0] += 0.03532767798156215;
          }
        } else {
          result[0] += 0.0119970394511896;
        }
      } else {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.0677499994635582109) ) ) {
          result[0] += 0.014230804582487025;
        } else {
          result[0] += 0.010285067397682835;
        }
      }
    } else {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02615000028163195003) ) ) {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.557041406631469949) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.0195500003173947369) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.183550000190734891) ) ) {
                result[0] += 0.015753873792944396;
              } else {
                result[0] += 0.013354723708824734;
              }
            } else {
              result[0] += 0.015181049237893263;
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1725500002503395358) ) ) {
              result[0] += 0.0110910349802209;
            } else {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2744500041007996161) ) ) {
                result[0] += 0.016622868751766703;
              } else {
                result[0] += 0.012466318187609527;
              }
            }
          }
        } else {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.470308303833008257) ) ) {
            if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.8250000178813935436) ) ) {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.006550000049173832807) ) ) {
                result[0] += 0.01115434647809036;
              } else {
                result[0] += 0.013331698361006863;
              }
            } else {
              result[0] += 0.01454336548237649;
            }
          } else {
            result[0] += 0.018893871310283136;
          }
        }
      } else {
        if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.3695499897003174383) ) ) {
          if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.05285000056028366783) ) ) {
            result[0] += 0.022039373994541502;
          } else {
            if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
              result[0] += 0.01502062323604185;
            } else {
              result[0] += 0.009418968575240827;
            }
          }
        } else {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2777500003576279242) ) ) {
            if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.8250000178813935436) ) ) {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02654999960213899959) ) ) {
                result[0] += 0.008908265806107972;
              } else {
                result[0] += 0.012676466008663258;
              }
            } else {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1416499987244606296) ) ) {
                result[0] += 0.012645590517443676;
              } else {
                result[0] += 0.00563795707216726;
              }
            }
          } else {
            result[0] += 0.017392378687632;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.029808140260210936;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.02738601624838264;
          } else {
            result[0] += 0.035001088559281746;
          }
        } else {
          result[0] += 0.010810127166993772;
        }
      } else {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01245000027120113546) ) ) {
          result[0] += 0.01840364366455435;
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8402999937534333386) ) ) {
            if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4910000115633011419) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2815500050783157904) ) ) {
                result[0] += 0.011123153301622309;
              } else {
                result[0] += 0.015520149424104502;
              }
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.593199998140335194) ) ) {
                result[0] += 0.006419995021077287;
              } else {
                result[0] += 0.011453332401026226;
              }
            }
          } else {
            result[0] += 0.016280210292109364;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07954999804496766524) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1157499998807907243) ) ) {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3600500077009201605) ) ) {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.08219999819993974166) ) ) {
              if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
                result[0] += 0.01546466621006001;
              } else {
                result[0] += 0.007869825287638145;
              }
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1587999984622001926) ) ) {
                result[0] += 0.015479425632761003;
              } else {
                result[0] += 0.0135077242073847;
              }
            }
          } else {
            if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7131499946117402233) ) ) {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6035499870777131237) ) ) {
                result[0] += 0.015950991899104195;
              } else {
                result[0] += 0.019182811034046586;
              }
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8007499873638154186) ) ) {
                result[0] += 0.007518921879466869;
              } else {
                result[0] += 0.012567863207982114;
              }
            }
          }
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3360500037670136053) ) ) {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.008450000081211330241) ) ) {
              result[0] += 0.005356382177994498;
            } else {
              result[0] += 0.011746225470942413;
            }
          } else {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02004999946802854885) ) ) {
              if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.474999994039535578) ) ) {
                result[0] += 0.014086037862219288;
              } else {
                result[0] += 0.01287628135872192;
              }
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4374500066041946966) ) ) {
                result[0] += 0.01521563183805794;
              } else {
                result[0] += 0.012286216703323121;
              }
            }
          }
        }
      } else {
        if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
          if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.09924999997019769149) ) ) {
            result[0] += 0.016429321023787104;
          } else {
            result[0] += 0.010774267241976117;
          }
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7549499869346619763) ) ) {
            result[0] += 0.023957540678348346;
          } else {
            result[0] += 0.01701522956579197;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.029693800609294475;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
          result[0] += 0.009191286403824554;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.698049992322921864) ) ) {
            result[0] += 0.02757294366680147;
          } else {
            result[0] += 0.03468433681425271;
          }
        }
      } else {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1544499993324280063) ) ) {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02145000081509352077) ) ) {
            result[0] += 0.01931129949206787;
          } else {
            result[0] += 0.011861836554425364;
          }
        } else {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02075000014156103481) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01725000049918890346) ) ) {
              result[0] += 0.011334868433744141;
            } else {
              result[0] += 0.00577015601232109;
            }
          } else {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2499499991536140719) ) ) {
              result[0] += 0.011026502160532014;
            } else {
              result[0] += 0.016867581726282867;
            }
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.2365819886326790134) ) ) {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01664999965578317989) ) ) {
          if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.06565000116825105148) ) ) {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.009550000075250865764) ) ) {
              result[0] += 0.012127217917826463;
            } else {
              result[0] += 0.007612558593064046;
            }
          } else {
            result[0] += 0.015658555770866753;
          }
        } else {
          if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.04065000079572201469) ) ) {
            result[0] += 0.014207486146100925;
          } else {
            result[0] += 0.019339708141289696;
          }
        }
      } else {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.08219999819993974166) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02615000028163195003) ) ) {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01515000034123659307) ) ) {
                result[0] += 0.013133103920260089;
              } else {
                result[0] += 0.015965259345176522;
              }
            } else {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02805000077933073391) ) ) {
                result[0] += 0.00843808141508431;
              } else {
                result[0] += 0.014795360312889055;
              }
            }
          } else {
            if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.04474999941885472038) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1725500002503395358) ) ) {
                result[0] += 0.01577099629268924;
              } else {
                result[0] += 0.012530450855222555;
              }
            } else {
              if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.05104999989271164634) ) ) {
                result[0] += 0.008210564036333518;
              } else {
                result[0] += 0.011892541478929918;
              }
            }
          }
        } else {
          if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2499499991536140719) ) ) {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1174499988555908342) ) ) {
                result[0] += 0.013300000963722976;
              } else {
                result[0] += 0.015263986858043631;
              }
            } else {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3079500049352646429) ) ) {
                result[0] += 0.011835389572545112;
              } else {
                result[0] += 0.013985548505067846;
              }
            }
          } else {
            if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.05525000020861626365) ) ) {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02805000077933073391) ) ) {
                result[0] += 0.014031920581216413;
              } else {
                result[0] += 0.008302778535758004;
              }
            } else {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.009949999861419202632) ) ) {
                result[0] += 0.006627868856444316;
              } else {
                result[0] += 0.011516533023113482;
              }
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.029583238998663088;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.02367744151651318;
          } else {
            result[0] += 0.034389428798955485;
          }
        } else {
          result[0] += 0.010695589136410695;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.01627982490080023;
        } else {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2437499985098839084) ) ) {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02375000063329935421) ) ) {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01395000005140900785) ) ) {
                result[0] += 0.016485063780115713;
              } else {
                result[0] += 0.00988742584624961;
              }
            } else {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02674999926239252437) ) ) {
                result[0] += 0.019152004476276116;
              } else {
                result[0] += 0.012028678090438043;
              }
            }
          } else {
            result[0] += 0.00566169423558555;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07954999804496766524) ) ) {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2466500028967857638) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3403999954462051947) ) ) {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1157499998807907243) ) ) {
                result[0] += 0.012910635852754829;
              } else {
                result[0] += 0.009714809006753157;
              }
            } else {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.08874999731779099899) ) ) {
                result[0] += 0.01632892679280136;
              } else {
                result[0] += 0.013438003521134508;
              }
            }
          } else {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2531500011682510931) ) ) {
              result[0] += 0.008629997820445672;
            } else {
              if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.03024999983608723034) ) ) {
                result[0] += 0.013727144176036855;
              } else {
                result[0] += 0.011092981541742857;
              }
            }
          }
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4944000095129013617) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3060500025749207098) ) ) {
              result[0] += 0.014223286393393505;
            } else {
              result[0] += 0.008624624329748459;
            }
          } else {
            result[0] += 0.018676071070552323;
          }
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04924999922513962486) ) ) {
          result[0] += 0.006649772654538514;
        } else {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02805000077933073391) ) ) {
            if ( LIKELY( !(data[15].missing != -1) || (data[15].fvalue <= (double)0.9388508796691895641) ) ) {
              if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.6304499804973603405) ) ) {
                result[0] += 0.013203908860204986;
              } else {
                result[0] += 0.010948635538587105;
              }
            } else {
              if ( LIKELY( !(data[15].missing != -1) || (data[15].fvalue <= (double)1.089584231376648171) ) ) {
                result[0] += 0.015368841777084277;
              } else {
                result[0] += 0.012246790118304821;
              }
            }
          } else {
            if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.04685000143945217826) ) ) {
              result[0] += 0.010228151040153602;
            } else {
              result[0] += 0.0060669427413573155;
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.029476302812168127;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.02536129333677328;
          } else {
            result[0] += 0.034102206442056825;
          }
        } else {
          result[0] += 0.009978370055809769;
        }
      } else {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01245000027120113546) ) ) {
          result[0] += 0.0179682679756222;
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1544499993324280063) ) ) {
            result[0] += 0.015001596796749159;
          } else {
            result[0] += 0.011722135853878005;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[10].missing != -1) || (data[10].fvalue <= (double)1.00000001800250948e-35) ) ) {
        if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2601500004529953558) ) ) {
          if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.8043499886989594616) ) ) {
            if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.760849982500076405) ) ) {
              result[0] += 0.01331182986768033;
            } else {
              result[0] += 0.016107824930473896;
            }
          } else {
            if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07744999974966050582) ) ) {
              if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.02185000013560057033) ) ) {
                result[0] += 0.010682798369038552;
              } else {
                result[0] += 0.013257643776062653;
              }
            } else {
              result[0] += 0.006307624938244441;
            }
          }
        } else {
          if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.01285000005736947233) ) ) {
            if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.09924999997019769149) ) ) {
              result[0] += 0.012912492765121597;
            } else {
              result[0] += 0.019207983942179836;
            }
          } else {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2670499980449677069) ) ) {
              if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.04894999973475933769) ) ) {
                result[0] += 0.007351045246310322;
              } else {
                result[0] += 0.0019267493766884178;
              }
            } else {
              if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.266450002789497431) ) ) {
                result[0] += 0.010420028428640218;
              } else {
                result[0] += 0.015884265576864984;
              }
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04924999922513962486) ) ) {
          if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.03024999983608723034) ) ) {
            result[0] += 0.012547005089701059;
          } else {
            result[0] += 0.006713765716744824;
          }
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1869500055909157077) ) ) {
            if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.03435000032186508873) ) ) {
              result[0] += 0.012167410261248192;
            } else {
              result[0] += 0.016916640460279353;
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2033500000834465304) ) ) {
              result[0] += 0.008347056104306813;
            } else {
              result[0] += 0.011939902933042673;
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.02937285307888511;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
            result[0] += 0.023763052618528197;
          } else {
            result[0] += 0.033828245375441636;
          }
        } else {
          result[0] += 0.010531617411154598;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.01625379149237042;
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8524500131607056774) ) ) {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
              result[0] += 0.006730697722804964;
            } else {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4797499924898148138) ) ) {
                result[0] += 0.014384574465829893;
              } else {
                result[0] += 0.010274023816491541;
              }
            }
          } else {
            result[0] += 0.01550446415742014;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2155499979853630343) ) ) {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03564999997615814903) ) ) {
          result[0] += 0.008633334694041777;
        } else {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1544499993324280063) ) ) {
            if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.8250000178813935436) ) ) {
              if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.625000000000000111) ) ) {
                result[0] += 0.012870712422006266;
              } else {
                result[0] += 0.010350255110869443;
              }
            } else {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01214999984949827368) ) ) {
                result[0] += 0.011062330998743588;
              } else {
                result[0] += 0.01575856463574618;
              }
            }
          } else {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01334999967366457159) ) ) {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01305000018328428442) ) ) {
                result[0] += 0.011218484307102624;
              } else {
                result[0] += 0.0047978244055375706;
              }
            } else {
              if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3739500045776367743) ) ) {
                result[0] += 0.012177556875999697;
              } else {
                result[0] += 0.010157537055417826;
              }
            }
          }
        }
      } else {
        if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5897500216960908093) ) ) {
          if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.381249994039535578) ) ) {
            if ( UNLIKELY( !(data[18].missing != -1) || (data[18].fvalue <= (double)0.6207582950592042126) ) ) {
              result[0] += 0.009097609332215623;
            } else {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01365000009536743338) ) ) {
                result[0] += 0.015742241396138532;
              } else {
                result[0] += 0.01333855000930153;
              }
            }
          } else {
            result[0] += 0.007945337222697624;
          }
        } else {
          if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.5250000059604645886) ) ) {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8499500155448914684) ) ) {
                result[0] += 0.012004038893637424;
              } else {
                result[0] += 0.016844391366221526;
              }
            } else {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02434999961405992855) ) ) {
                result[0] += 0.012637051156585028;
              } else {
                result[0] += 0.009572433744839848;
              }
            }
          } else {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01515000034123659307) ) ) {
              result[0] += 0.004336885015816664;
            } else {
              result[0] += 0.009660051185681053;
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.029272756642450663;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3750000000000000555) ) ) {
          result[0] += 0.004453082106364893;
        } else {
          if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
            result[0] += 0.03356327494721765;
          } else {
            result[0] += 0.014582166939691511;
          }
        }
      } else {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1692000031471252719) ) ) {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1544499993324280063) ) ) {
            result[0] += 0.015759238933724364;
          } else {
            if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.01994999963790178646) ) ) {
              result[0] += 0.006598021238877011;
            } else {
              result[0] += 0.014010768623634404;
            }
          }
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.698049992322921864) ) ) {
            result[0] += 0.006776675201724881;
          } else {
            result[0] += 0.011945006149673757;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
        if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2606499940156937201) ) ) {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2262500002980232516) ) ) {
            if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2514500021934509832) ) ) {
                result[0] += 0.013306405997196123;
              } else {
                result[0] += 0.011113944705911056;
              }
            } else {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03714999929070473411) ) ) {
                result[0] += 0.00596430641739077;
              } else {
                result[0] += 0.012787825786621397;
              }
            }
          } else {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2863499969244003851) ) ) {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.844049990177154652) ) ) {
                result[0] += 0.012800033493239316;
              } else {
                result[0] += 0.007688006095282882;
              }
            } else {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3152499943971634466) ) ) {
                result[0] += 0.0010377901590705994;
              } else {
                result[0] += 0.010312515930379204;
              }
            }
          }
        } else {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.293586850166321245) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3123999983072281439) ) ) {
              result[0] += 0.01033993920984739;
            } else {
              result[0] += 0.013410887880458078;
            }
          } else {
            result[0] += 0.015387844345753469;
          }
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1491499990224838534) ) ) {
          result[0] += 0.006606445959908698;
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7681500017642975964) ) ) {
            if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07324999943375588851) ) ) {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5602000057697297253) ) ) {
                result[0] += 0.011569125469026297;
              } else {
                result[0] += 0.013830220031327225;
              }
            } else {
              if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07744999974966050582) ) ) {
                result[0] += 0.003731406390018679;
              } else {
                result[0] += 0.010378630417323488;
              }
            }
          } else {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.1750000044703483859) ) ) {
              result[0] += 0.015242151004283767;
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.844049990177154652) ) ) {
                result[0] += 0.004843002976735;
              } else {
                result[0] += 0.009788936712686613;
              }
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.029175883218018436;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01035000011324882681) ) ) {
        if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
          result[0] += 0.004453037280713235;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.698049992322921864) ) ) {
            result[0] += 0.025343269967143395;
          } else {
            result[0] += 0.03330632816654569;
          }
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2931499928236008246) ) ) {
          result[0] += 0.007352454198593202;
        } else {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2201500013470649997) ) ) {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.330050006508827265) ) ) {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1174499988555908342) ) ) {
                result[0] += 0.01607664530922531;
              } else {
                result[0] += 0.010250343718522277;
              }
            } else {
              result[0] += 0.016684208256736253;
            }
          } else {
            result[0] += 0.009292815681600716;
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02634999994188547481) ) ) {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2879499942064285833) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.128800004720687894) ) ) {
              result[0] += 0.008633626653044166;
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.1587999984622001926) ) ) {
                result[0] += 0.0177131226931838;
              } else {
                result[0] += 0.01248738340720643;
              }
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.9198000133037568249) ) ) {
              result[0] += 0.004538116006081199;
            } else {
              result[0] += 0.013099754319349961;
            }
          }
        } else {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2333500012755394259) ) ) {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
              result[0] += 0.008899039491167113;
            } else {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5788500010967255749) ) ) {
                result[0] += 0.010642572336455498;
              } else {
                result[0] += 0.015803885872270638;
              }
            }
          } else {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02845000009983778347) ) ) {
              result[0] += 0.0021107161141314572;
            } else {
              result[0] += 0.007911186520613093;
            }
          }
        }
      } else {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02654999960213899959) ) ) {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.339049994945526345) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.194349996745586423) ) ) {
              if ( UNLIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.2916499972343445379) ) ) {
                result[0] += 0.007640236676421823;
              } else {
                result[0] += 0.01407399042847985;
              }
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3271500021219254095) ) ) {
                result[0] += 0.008433246171798128;
              } else {
                result[0] += 0.010764497882052958;
              }
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2339999973773956576) ) ) {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02004999946802854885) ) ) {
                result[0] += 0.006497401907668903;
              } else {
                result[0] += 0.01250387437901741;
              }
            } else {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02495000045746565212) ) ) {
                result[0] += 0.012313152399818185;
              } else {
                result[0] += 0.007889129264465595;
              }
            }
          }
        } else {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2777500003576279242) ) ) {
            if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.796943068504333718) ) ) {
              if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.082304060459137185) ) ) {
                result[0] += 0.011202860295058419;
              } else {
                result[0] += 0.013698171212875282;
              }
            } else {
              result[0] += 0.008538834413422828;
            }
          } else {
            result[0] += 0.01720992223646337;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.029082108369161825;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01035000011324882681) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.698049992322921864) ) ) {
            result[0] += 0.02066301850917822;
          } else {
            result[0] += 0.033069245482289476;
          }
        } else {
          result[0] += 0.010207649332615984;
        }
      } else {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1544499993324280063) ) ) {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02165000047534704555) ) ) {
            result[0] += 0.016788186359033137;
          } else {
            result[0] += 0.010573864256765878;
          }
        } else {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2155499979853630343) ) ) {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02004999946802854885) ) ) {
              if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
                result[0] += 0.003859284846863016;
              } else {
                result[0] += 0.011261151608837816;
              }
            } else {
              if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.07615000009536744552) ) ) {
                result[0] += 0.011834732025368402;
              } else {
                result[0] += 0.016993740635800888;
              }
            }
          } else {
            result[0] += 0.005311525380518214;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.557041406631469949) ) ) {
        if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.0359499994665384362) ) ) {
          if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.04274999909102917411) ) ) {
            result[0] += 0.0037793056567337587;
          } else {
            result[0] += 0.009968492886598016;
          }
        } else {
          if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07954999804496766524) ) ) {
            if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.05285000056028366783) ) ) {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.06284999847412110763) ) ) {
                result[0] += 0.014288675662793427;
              } else {
                result[0] += 0.011131334138452494;
              }
            } else {
              if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.1750000044703483859) ) ) {
                result[0] += 0.00882512738823069;
              } else {
                result[0] += 0.010666808650047802;
              }
            }
          } else {
            if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.08874999731779099899) ) ) {
                result[0] += 0.01459500366764;
              } else {
                result[0] += 0.008967527010477393;
              }
            } else {
              if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.1430500000715256015) ) ) {
                result[0] += 0.020142991941342822;
              } else {
                result[0] += 0.012410266475124937;
              }
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2105000019073486606) ) ) {
          if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3006500005722046454) ) ) {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2056499943137169162) ) ) {
              result[0] += 0.0084743122509775;
            } else {
              result[0] += 0.01385509261183739;
            }
          } else {
            result[0] += 0.005796119288727562;
          }
        } else {
          if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.625000000000000111) ) ) {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1157499998807907243) ) ) {
              if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
                result[0] += 0.01654086029088239;
              } else {
                result[0] += 0.012586495121862353;
              }
            } else {
              if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2750000059604645331) ) ) {
                result[0] += 0.010864276004750073;
              } else {
                result[0] += 0.012926502326477289;
              }
            }
          } else {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1174499988555908342) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5023500025272370495) ) ) {
                result[0] += 0.002240515059914309;
              } else {
                result[0] += 0.00870892923734843;
              }
            } else {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.936650007963180653) ) ) {
                result[0] += 0.012900904394494478;
              } else {
                result[0] += 0.008177993847417133;
              }
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.02899132267988798;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6658999919891358532) ) ) {
          result[0] += 0.006538513296138199;
        } else {
          result[0] += 0.03282694145240286;
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.01490628658373883;
        } else {
          if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.796943068504333718) ) ) {
            if ( UNLIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.2916499972343445379) ) ) {
              result[0] += 0.006893869156306973;
            } else {
              if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
                result[0] += 0.007524451778055879;
              } else {
                result[0] += 0.01374133208127961;
              }
            }
          } else {
            result[0] += 0.004473849007185086;
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03415000066161156395) ) ) {
        result[0] += 0.00694541738811635;
      } else {
        if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.5250000059604645886) ) ) {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2940499931573868353) ) ) {
            if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.2365819886326790134) ) ) {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.08874999731779099899) ) ) {
                result[0] += 0.016407765783096714;
              } else {
                result[0] += 0.012079508305595652;
              }
            } else {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2466500028967857638) ) ) {
                result[0] += 0.011034425686550258;
              } else {
                result[0] += 0.010095608207623513;
              }
            }
          } else {
            result[0] += 0.014086617658576578;
          }
        } else {
          if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.413049995899200495) ) ) {
            result[0] += 0.01322099431247629;
          } else {
            if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.9347499907016755261) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2230000048875808993) ) ) {
                result[0] += 0.005962106602121993;
              } else {
                result[0] += 0.009764410887939936;
              }
            } else {
              result[0] += 0.004242769904680846;
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.0289034056309581;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6525499820709229626) ) ) {
          result[0] += 0.008114275635031669;
        } else {
          result[0] += 0.032604053407323724;
        }
      } else {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01245000027120113546) ) ) {
          result[0] += 0.015069278440375406;
        } else {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.9387499988079072155) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3714499920606613714) ) ) {
              result[0] += 0.012495597312640975;
            } else {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01894999947398901333) ) ) {
                result[0] += 0.0043156539693964095;
              } else {
                result[0] += 0.009015866046756716;
              }
            }
          } else {
            result[0] += 0.01329628648004565;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.470308303833008257) ) ) {
        if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.5434499979019166149) ) ) {
          if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03875000029802323082) ) ) {
            result[0] += 0.014614932568193708;
          } else {
            if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.08219999819993974166) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.4441500008106232245) ) ) {
                result[0] += 0.006505005288812898;
              } else {
                result[0] += 0.011371130989160497;
              }
            } else {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6443499922752381481) ) ) {
                result[0] += 0.011715251981412904;
              } else {
                result[0] += 0.010750218296471652;
              }
            }
          }
        } else {
          if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.128800004720687894) ) ) {
              result[0] += 0.005564363970745219;
            } else {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3834999948740006048) ) ) {
                result[0] += 0.012243240361182503;
              } else {
                result[0] += 0.01059269879657454;
              }
            }
          } else {
            if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3403999954462051947) ) ) {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2967500090599060614) ) ) {
                result[0] += 0.00854469907790595;
              } else {
                result[0] += 0.005923311583286213;
              }
            } else {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6889500021934510388) ) ) {
                result[0] += 0.011667041382851986;
              } else {
                result[0] += 0.00912409138730449;
              }
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
          result[0] += 0.009282429435105063;
        } else {
          result[0] += 0.01994636286905987;
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.028818256171705043;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01004999969154596502) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.698049992322921864) ) ) {
            result[0] += 0.019157711048276908;
          } else {
            result[0] += 0.03239779374833309;
          }
        } else {
          result[0] += 0.007651335638684454;
        }
      } else {
        if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.8402999937534333386) ) ) {
          if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6843999922275544323) ) ) {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
              result[0] += 0.015014858061615104;
            } else {
              if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5213499963283539929) ) ) {
                result[0] += 0.011003235064231436;
              } else {
                result[0] += 0.0066172969457374674;
              }
            }
          } else {
            result[0] += 0.005001305204603827;
          }
        } else {
          result[0] += 0.013598007764497523;
        }
      }
    } else {
      if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)0.8623363375663758434) ) ) {
        if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02615000028163195003) ) ) {
          if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.0178499994799494778) ) ) {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.06284999847412110763) ) ) {
              result[0] += 0.012486104059884392;
            } else {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2879499942064285833) ) ) {
                result[0] += 0.008416354068854923;
              } else {
                result[0] += 0.011866219756497152;
              }
            }
          } else {
            if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02495000045746565212) ) ) {
              if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.01845000032335520138) ) ) {
                result[0] += 0.014953113390208302;
              } else {
                result[0] += 0.010667041009230086;
              }
            } else {
              result[0] += 0.013991990934053388;
            }
          }
        } else {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2531500011682510931) ) ) {
            if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02674999926239252437) ) ) {
              result[0] += 0.004505166831111649;
            } else {
              result[0] += 0.009643409221511117;
            }
          } else {
            result[0] += 0.0027572327697014225;
          }
        }
      } else {
        if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.006150000030174852285) ) ) {
          if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.005349999992176891241) ) ) {
            result[0] += 0.012608688212375674;
          } else {
            if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2041499987244606296) ) ) {
              if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5057500004768372692) ) ) {
                result[0] += 0.004232664626281799;
              } else {
                result[0] += 0.012146555506963485;
              }
            } else {
              result[0] += 0.0034971664802535543;
            }
          }
        } else {
          if ( UNLIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.009449999779462816066) ) ) {
            if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.9347499907016755261) ) ) {
              if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.082304060459137185) ) ) {
                result[0] += 0.009102301338719632;
              } else {
                result[0] += 0.012692868123755611;
              }
            } else {
              result[0] += 0.006433645789951833;
            }
          } else {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.04924999922513962486) ) ) {
              if ( LIKELY( !(data[13].missing != -1) || (data[13].fvalue <= (double)0.02004999946802854885) ) ) {
                result[0] += 0.005022221501787534;
              } else {
                result[0] += 0.011925227629466087;
              }
            } else {
              if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.266450002789497431) ) ) {
                result[0] += 0.010588352756874761;
              } else {
                result[0] += 0.011918525379755233;
              }
            }
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.02873577276220743;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.4583500027656555731) ) ) {
          result[0] += 0.00603041868139955;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6889500021934510388) ) ) {
            result[0] += 0.015393951784662255;
          } else {
            result[0] += 0.03218948030991773;
          }
        }
      } else {
        if ( UNLIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.07615000009536744552) ) ) {
          result[0] += 0.014572598310227317;
        } else {
          if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.04274999909102917411) ) ) {
            result[0] += 0.011489130938476092;
          } else {
            result[0] += 0.00808226814810115;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.9347499907016755261) ) ) {
        if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.0753500014543533464) ) ) {
          if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.2750000059604645331) ) ) {
            if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)2.130882143974304643) ) ) {
              if ( UNLIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.082304060459137185) ) ) {
                result[0] += 0.0077815823484347285;
              } else {
                result[0] += 0.0098914156798227;
              }
            } else {
              if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.01285000005736947233) ) ) {
                result[0] += 0.009849726459006445;
              } else {
                result[0] += 0.017711836162341444;
              }
            }
          } else {
            if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.7019500136375428356) ) ) {
              if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07324999943375588851) ) ) {
                result[0] += 0.011043713873173659;
              } else {
                result[0] += 0.0062256532946723606;
              }
            } else {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1157499998807907243) ) ) {
                result[0] += 0.004100747012150571;
              } else {
                result[0] += 0.009977815746299221;
              }
            }
          }
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.3453000038862228949) ) ) {
            if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2776999920606613714) ) ) {
              if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3750000000000000555) ) ) {
                result[0] += 0.015031352260164344;
              } else {
                result[0] += 0.005229814429065023;
              }
            } else {
              result[0] += 0.004750789806288426;
            }
          } else {
            if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
              result[0] += 0.013573591445287534;
            } else {
              result[0] += 0.009996195272652407;
            }
          }
        }
      } else {
        if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1961499974131584445) ) ) {
          if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3152499943971634466) ) ) {
            if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.03435000032186508873) ) ) {
              if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.005249999929219485197) ) ) {
                result[0] += 0.01340303288838866;
              } else {
                result[0] += 0.004264196678224297;
              }
            } else {
              result[0] += 0.013669564291595708;
            }
          } else {
            result[0] += 0.004440939201836906;
          }
        } else {
          if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2041499987244606296) ) ) {
            if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2269499972462654391) ) ) {
              result[0] += -0.00012083172066981775;
            } else {
              if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2279499992728233615) ) ) {
                result[0] += 0.0028706301528427366;
              } else {
                result[0] += 0.008645516919874826;
              }
            }
          } else {
            result[0] += 0.01071668786061811;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.02865585860500651;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.625000000000000111) ) ) {
          result[0] += 0.007035903773411;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6889500021934510388) ) ) {
            result[0] += 0.017438520749827754;
          } else {
            result[0] += 0.03199070263402186;
          }
        }
      } else {
        if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.2514500021934509832) ) ) {
          result[0] += 0.005601919419951774;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.5132499933242798962) ) ) {
            if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1416499987244606296) ) ) {
              result[0] += 0.014987874679441104;
            } else {
              result[0] += 0.00909291488646013;
            }
          } else {
            result[0] += 0.009692562218716947;
          }
        }
      }
    } else {
      if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.03995000012218952873) ) ) {
        if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.4565500020980835516) ) ) {
          result[0] += 0.012319957734263531;
        } else {
          if ( UNLIKELY( !(data[11].missing != -1) || (data[11].fvalue <= (double)1.00000001800250948e-35) ) ) {
            result[0] += 0.0015957367751710352;
          } else {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
              result[0] += 0.011729876579137927;
            } else {
              result[0] += 0.0034610424229122878;
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
          if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
            if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.1174499988555908342) ) ) {
                result[0] += 0.011606828926990392;
              } else {
                result[0] += 0.00719359564662948;
              }
            } else {
              if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.474999994039535578) ) ) {
                result[0] += 0.011076241461291573;
              } else {
                result[0] += 0.009517048261816789;
              }
            }
          } else {
            if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.02185000013560057033) ) ) {
              if ( UNLIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.2129499986767769137) ) ) {
                result[0] += 0.011403926988078236;
              } else {
                result[0] += 0.008052169103550954;
              }
            } else {
              if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.0677499994635582109) ) ) {
                result[0] += 0.012418013150056544;
              } else {
                result[0] += 0.00988017890215503;
              }
            }
          }
        } else {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2950499951839447577) ) ) {
            if ( LIKELY( !(data[7].missing != -1) || (data[7].fvalue <= (double)0.3079500049352646429) ) ) {
              if ( LIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3750000000000000555) ) ) {
                result[0] += 0.00997740309928194;
              } else {
                result[0] += 0.008400761808607741;
              }
            } else {
              if ( UNLIKELY( !(data[8].missing != -1) || (data[8].fvalue <= (double)0.3250000029802322943) ) ) {
                result[0] += 0.008824165087029597;
              } else {
                result[0] += 0.011741279579578911;
              }
            }
          } else {
            result[0] += 0.014790177840354095;
          }
        }
      }
    }
  }
  if ( LIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.0662999991327524324) ) ) {
    result[0] += -0.02857841917430312;
  } else {
    if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.2826000005006790716) ) ) {
      if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
        if ( UNLIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.4583500027656555731) ) ) {
          result[0] += 0.0069359144274197026;
        } else {
          if ( UNLIKELY( !(data[2].missing != -1) || (data[2].fvalue <= (double)0.6796000003814698376) ) ) {
            result[0] += 0.011100754957309802;
          } else {
            result[0] += 0.031799153593534094;
          }
        }
      } else {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          result[0] += 0.013363200677913264;
        } else {
          if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3250000029802322943) ) ) {
            result[0] += 0.004584370050364925;
          } else {
            result[0] += 0.010452949958434434;
          }
        }
      }
    } else {
      if ( LIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.4583500027656555731) ) ) {
        if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.2250000014901161471) ) ) {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2744500041007996161) ) ) {
            if ( UNLIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.5434499979019166149) ) ) {
              if ( LIKELY( !(data[3].missing != -1) || (data[3].fvalue <= (double)0.5000000149011613049) ) ) {
                result[0] += 0.009429999619239715;
              } else {
                result[0] += 0.01623125147281992;
              }
            } else {
              if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1416499987244606296) ) ) {
                result[0] += 0.010491318409129952;
              } else {
                result[0] += 0.0065634965409253326;
              }
            }
          } else {
            result[0] += 0.005038442815422861;
          }
        } else {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1684500053524971286) ) ) {
            if ( LIKELY( !(data[1].missing != -1) || (data[1].fvalue <= (double)0.3750000000000000555) ) ) {
              if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.005949999904260040197) ) ) {
                result[0] += 0.006959395334583341;
              } else {
                result[0] += 0.009284291371438968;
              }
            } else {
              if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.1416499987244606296) ) ) {
                result[0] += 0.012263484769484355;
              } else {
                result[0] += 0.007943363267400479;
              }
            }
          } else {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
              if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.04274999909102917411) ) ) {
                result[0] += 0.012887579156475721;
              } else {
                result[0] += 0.010455395361608113;
              }
            } else {
              if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.07115000113844872909) ) ) {
                result[0] += 0.010195394294926444;
              } else {
                result[0] += 0.013148652270159163;
              }
            }
          }
        }
      } else {
        if ( UNLIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.006599999964237214002) ) ) {
          if ( UNLIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.06284999847412110763) ) ) {
            result[0] += 0.014675246074699307;
          } else {
            if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.111188352108001931) ) ) {
              result[0] += 0.01011168507276173;
            } else {
              result[0] += 0.014051874392476276;
            }
          }
        } else {
          if ( LIKELY( !(data[0].missing != -1) || (data[0].fvalue <= (double)0.2466500028967857638) ) ) {
            if ( LIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.3999999910593033392) ) ) {
              if ( LIKELY( !(data[14].missing != -1) || (data[14].fvalue <= (double)1.111188352108001931) ) ) {
                result[0] += 0.011299981942348507;
              } else {
                result[0] += 0.00960605372096199;
              }
            } else {
              if ( LIKELY( !(data[6].missing != -1) || (data[6].fvalue <= (double)0.2553499937057495672) ) ) {
                result[0] += 0.007785878368071161;
              } else {
                result[0] += 0.01038867075037273;
              }
            }
          } else {
            if ( UNLIKELY( !(data[12].missing != -1) || (data[12].fvalue <= (double)0.1750000044703483859) ) ) {
              result[0] += 0.01270663813201077;
            } else {
              if ( LIKELY( !(data[5].missing != -1) || (data[5].fvalue <= (double)0.0753500014543533464) ) ) {
                result[0] += 0.005563945070231304;
              } else {
                result[0] += 0.011498931916819166;
              }
            }
          }
        }
      }
    }
  }
}


/* Wrapper para o vetor de 19 features do main.c */
static inline double predict_fraud_lgb(const double *features) {
    union Entry input[19];
    for (int i = 0; i < 19; i++) {
        input[i].fvalue = features[i]; // Sem cast para float!
        input[i].missing = 0;          // 0 indica que o valor está presente
    }
    
    // CRÍTICO: Inicializar com 0.0 pois o modelo faz acumulação
    double out_result[1] = {0.0}; 
    
    predict(input, 0, out_result); 
    return out_result[0];
}
