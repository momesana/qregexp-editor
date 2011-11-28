/*
 * Copyright (C) 2011 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * This file is part of QRegExp-Editor.
 *
 * QRegExp-Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QRegExp-Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegExp-Editor. If not, see <http://www.gnu.org/licenses/>.
 */

#include "regexpoptions.h"

RegexpOptions::RegexpOptions()
    : showTabsAndSpaces(false),
      showNewlines(false),
      showParenthesesMatch(true)
{
}

bool RegexpOptions::equals(const RegexpOptions &s) const
{
    return showTabsAndSpaces == s.showTabsAndSpaces
           && showNewlines == s.showNewlines
           && showParenthesesMatch == s.showParenthesesMatch;
}

QDebug operator<<(QDebug dbg, const RegexpOptions &p)
{
    dbg.nospace() << "(" << p.showTabsAndSpaces << ", " <<
                  p.showNewlines << ", " <<
                  p.showParenthesesMatch << ")";
    return dbg.space();
}
