/*
 * Copyright © 2015 Christian Persch
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <pango/pango.h>
#include <gdk/gdk.h>

namespace vte {

namespace grid {

        typedef long row_t;
        typedef long column_t;

        struct coords {
        public:
                coords() = default;
                coords(row_t r, column_t c) : m_row(r), m_column(c) { }

                inline void set_row(row_t r)       { m_row = r; }
                inline void set_column(column_t c) { m_column = c; }

                inline row_t row()       const { return m_row; }
                inline column_t column() const { return m_column; }

                inline bool operator == (coords const& rhs) const { return m_row == rhs.m_row && m_column == rhs.m_column; }
                inline bool operator != (coords const& rhs) const { return m_row != rhs.m_row || m_column != rhs.m_column; }

                inline bool operator <  (coords const& rhs) const { return m_row < rhs.m_row || (m_row == rhs.m_row && m_column <  rhs.m_column); }
                inline bool operator <= (coords const& rhs) const { return m_row < rhs.m_row || (m_row == rhs.m_row && m_column <= rhs.m_column); }
                inline bool operator >  (coords const& rhs) const { return m_row > rhs.m_row || (m_row == rhs.m_row && m_column >  rhs.m_column); }
                inline bool operator >= (coords const& rhs) const { return m_row > rhs.m_row || (m_row == rhs.m_row && m_column >= rhs.m_column); }

        private:
                row_t m_row;
                column_t m_column;
        };

        struct span {
        public:
                span() = default;
                span(coords const& s, coords const& e) : m_start(s), m_end(e) { }
                span(row_t sr, column_t sc, row_t er, column_t ec) : m_start(sr, sc), m_end(er, ec) { }

                inline void set_start(coords const& s) { m_start = s; }
                inline void set_end(coords const& e) { m_end = e; }

                inline bool operator == (span const& rhs) const { return m_start == rhs.m_start && m_end == rhs.m_end; }
                inline bool operator != (span const& rhs) const { return m_start != rhs.m_start || m_end != rhs.m_end; }

                inline coords const& start() const { return m_start; }
                inline coords const& end()   const { return m_end; }
                inline row_t start_row()       const { return m_start.row(); }
                inline row_t end_row()         const { return m_end.row(); }
                inline column_t start_column() const { return m_start.column(); }
                inline column_t end_column()   const { return m_end.column(); }

                inline void clear() { m_start = coords(-1, -1); m_end = coords(-2, -2); }
                inline bool empty() const { return m_start > m_end; }
                inline explicit operator bool() const { return !empty(); }

                inline bool contains(coords const& p) const { return m_start <= p && p <= m_end; }
                inline bool box_contains(coords const& p) const { return m_start.row() <= p.row() && p.row() <= m_end.row() &&
                                                                         m_start.column() <= p.column() && p.column() <= m_end.column(); }

        private:
                coords m_start;
                coords m_end;
        };

} /* namespace grid */


namespace view {

        /* FIXMEchpe: actually 32-bit int would be sufficient here */
        typedef long coord_t;

        struct coords {
        public:
                coords() = default;
                coords(coord_t x_, coord_t y_) : x(x_), y(y_) { }

                inline bool operator == (coords const& rhs) const { return x == rhs.x && y == rhs.y; }
                inline bool operator != (coords const& rhs) const { return x != rhs.x || y != rhs.y; }

                void swap(coords &rhs) { coords tmp = rhs; rhs = *this; *this = tmp; }
        public:
                coord_t x;
                coord_t y;
};

} /* namespace view */

namespace color {

        /* 24-bit (8 bit per channel) packed colour */
        /* FIXME: we could actually support 10 bit per channel */
        typedef guint32 packed;

        class rgb : public PangoColor {
        public:
                rgb() = default;
                rgb(PangoColor const& c) { *static_cast<PangoColor*>(this) = c; }
                rgb(GdkRGBA const* c);

                rgb(rgb const& a, rgb const& b, double f);
                rgb(rgb const* a, rgb const* b, double f) : rgb(*a, *b, f) { }

                bool parse(char const* spec);

                void from_pango(PangoColor const& c) { *static_cast<PangoColor*>(this) = c; }

                inline bool operator == (rgb const& rhs) const {
                        return red == rhs.red && green == rhs.green && blue == rhs.blue;
                }
        };

} /* namespace color */

} /* namespace vte */