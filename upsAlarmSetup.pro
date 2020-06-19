# Copyright (C) 2020  Gabriele Salvato

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


QT += core
QT += gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = upsAlarmSetup
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000


SOURCES += main.cpp
SOURCES += configuredialog.cpp


HEADERS  += configuredialog.h
