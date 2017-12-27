#include <catch.hpp>
#include <stack.hpp>
#include <iostream>
#include <mutex>
#include <memory>
#include <stdexcept>

SCENARIO("stack init")
{
	stack<int> st;
	REQUIRE(st.count() == 0);
}

SCENARIO("stack init object")
{
	stack<int> st1;

	st1.push(1);
	st1.push(2);
	st1.push(3);
	stack<int> st2(st1);
	REQUIRE(st1.count() == 3);
	REQUIRE(st1.count() == st2.count());
}

SCENARIO("stack op=")
{
	stack<int> st1;

	st1.push(1);
	st1.push(2);
	st1.push(3);
	stack<int> st2 = st1;
	REQUIRE(st1.count() == 3);
	REQUIRE(st1.count() == st2.count());
}

SCENARIO("stack push")
{
	stack<int> st;
	REQUIRE(st.count() == 0);
	st.push(6);
	REQUIRE(st.count() == 1);
	st.push(7);
	REQUIRE(st.count() == 2);
	st.push(78);
	REQUIRE(st.count() == 3);
}

SCENARIO("pop")
{
	stack<int> st;
	st.push(1);
	st.push(2);
	st.push(3);
	int val = *st.pop();
	REQUIRE(st.count() == 2);
	REQUIRE(val == 3);
	val = *st.pop();
	REQUIRE(st.count() == 1);
	REQUIRE(val == 1);
	val = *st.pop();
	REQUIRE(st.count() == 0);
	REQUIRE(val == 1);
}
