/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @author Christian <c@ethdev.com>
 * @date 2015
 * Tests for high level features like import.
 */

#include <string>
#include <boost/test/unit_test.hpp>
#include <libsolidity/interface/Exceptions.h>
#include <libsolidity/interface/CompilerStack.h>

using namespace std;

namespace dev
{
namespace solidity
{
namespace test
{

BOOST_AUTO_TEST_SUITE(SolidityImports)

BOOST_AUTO_TEST_CASE(smoke_test)
{
	CompilerStack c;
	c.addSource("a", "contract C {}");
	BOOST_CHECK(c.compile());
}

BOOST_AUTO_TEST_CASE(regular_import)
{
	CompilerStack c;
	c.addSource("a", "contract C {}");
	c.addSource("b", "import \"a\"; contract D is C {}");
	BOOST_CHECK(c.compile());
}

BOOST_AUTO_TEST_CASE(import_does_not_clutter_importee)
{
	CompilerStack c;
	c.addSource("a", "contract C { D d; }");
	c.addSource("b", "import \"a\"; contract D is C {}");
	BOOST_CHECK(!c.compile());
}

BOOST_AUTO_TEST_CASE(import_is_transitive)
{
	CompilerStack c;
	c.addSource("a", "contract C { }");
	c.addSource("b", "import \"a\";");
	c.addSource("c", "import \"b\"; contract D is C {}");
	BOOST_CHECK(c.compile());
}

BOOST_AUTO_TEST_CASE(circular_import)
{
	CompilerStack c;
	c.addSource("a", "import \"b\"; contract C { D d; }");
	c.addSource("b", "import \"a\"; contract D { C c; }");
	BOOST_CHECK(c.compile());
}

BOOST_AUTO_TEST_CASE(relative_import)
{
	CompilerStack c;
	c.addSource("a", "import \"./dir/b\"; contract A is B {}");
	c.addSource("dir/b", "contract B {}");
	c.addSource("dir/c", "import \"../a\"; contract C is A {}");
	BOOST_CHECK(c.compile());
}

BOOST_AUTO_TEST_CASE(relative_import_multiplex)
{
	CompilerStack c;
	c.addSource("a", "contract A {}");
	c.addSource("dir/a/b/c", "import \"../../.././a\"; contract B is A {}");
	BOOST_CHECK(c.compile());
}

BOOST_AUTO_TEST_SUITE_END()

}
}
} // end namespaces
