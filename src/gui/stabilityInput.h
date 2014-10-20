#ifndef STABILITYINPUT_H
#define STABILITYINPUT_H

#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QGridLayout>

#include <string>
#include <vector>

#include "gdal_priv.h"
#include "ogr_srs_api.h"

#include "boost/date_time/local_time/local_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "latLonWidget.h"
#include "timeZoneWidget.h"

#include "qdebug.h"

class stabilityInput : public QWidget
{
  Q_OBJECT

 public:
  
  stabilityInput(QWidget *parent = 0);
  QGroupBox *stabilityGroupBox;
  QVBoxLayout *stabilityLayout;
  QVBoxLayout *layout;

};

#endif /* STABILITYINPUT_H */