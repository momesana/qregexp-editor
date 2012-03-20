/*
 * Copyright (C) 2012 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * This file is part of QRegexp-Editor.
 *
 * QRegexp-Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QRegExp-Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegexp-Editor. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PATTERNFILTERTEST_H
#define PATTERNFILTERTEST_H

class PatternFilter;
#include <QtCore/QObject>

class PatternFilterTest : public QObject {
    Q_OBJECT

public:
    PatternFilterTest();

private slots:
    void init();
    void cleanupTestCase();

    void testNoFilters_data();
    void testNoFilters();
    void testNewlines_data();
    void testNewlines();
    void testTrailingWhitespaces_data();
    void testTrailingWhitespaces();
    void testNewlinesAndTrailingWhitespaces_data();
    void testNewlinesAndTrailingWhitespaces();

private:
    PatternFilter *pf;
};

#endif // PATTERNFILTERTEST_H
