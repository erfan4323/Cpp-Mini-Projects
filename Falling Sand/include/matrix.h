#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <fmt/core.h>
#include <fmt/ranges.h>

enum Direction
{
	UP = 0,
	DOWN = 1 << 0,
	LEFT = 1 << 1,
	RIGHT = 1 << 2,
	RIGHT_DOWN = 1 << 3,
	LEFT_DOWN = 1 << 4,
	RIGHT_UP = 1 << 5,
	LEFT_UP = 1 << 6
};

template <class T>
class matrix
{
private:
	std::vector<std::vector<T>> data;
	int rows;
	int cols;

public:
	// Proxy class to handle element access and movement
	class Proxy
	{
	private:
		matrix<T>& mat;
		int r;
		int c;

		inline void SwitchIndexes(int row, int col)
		{
			if (row >= 0 && row < mat.rows && col >= 0 && col < mat.cols)
			{
				std::swap(mat.data[r][c], mat.data[row][col]);
				r = row;
				c = col;
			}
			else
			{
				throw std::out_of_range("SwitchIndexes: Index out of range");
			}
		}

		void ChangeLocation(Direction dir, int& new_r, int& new_c) const
		{
			switch (dir)
			{
			case UP:    new_r = r - 1; break;
			case DOWN:  new_r = r + 1; break;
			case LEFT:  new_c = c - 1; break;
			case RIGHT: new_c = c + 1; break;
			case RIGHT_DOWN:    new_r = r + 1; new_c = c + 1; break;
			case LEFT_DOWN:     new_r = r + 1; new_c = c - 1; break;
			case RIGHT_UP:      new_r = r - 1; new_c = c + 1; break;
			case LEFT_UP:       new_r = r - 1; new_c = c - 1; break;
			default: throw std::invalid_argument("Invalid direction");
			}
		}

	public:
		Proxy(matrix<T>& mat, int r, int c) : mat(mat), r(r), c(c)
		{
			if (r >= mat.rows || c >= mat.cols)
				throw std::out_of_range("Index out of range");
		}

		Proxy& operator=(const T& value)
		{
			mat.data[r][c] = value;
			return *this;
		}

		inline operator T() const
		{
			return mat.data[r][c];
		}

		Proxy& Move(Direction dir)
		{
			int new_r = r, new_c = c;

			ChangeLocation(dir, new_r, new_c);

			SwitchIndexes(new_r, new_c);

			return *this;
		}

		T index(Direction dir) const
		{
			int new_r = r, new_c = c;

			ChangeLocation(dir, new_r, new_c);

			if (new_r >= 0 && new_r < mat.Rows() && new_c >= 0 && new_c < mat.Cols())
			{
				return mat.data[new_r][new_c];
			}
		}

		T& get()
		{
			return mat.data[r][c];
		}

		const T& get() const
		{
			return mat.data[r][c];
		}
	};

	#pragma region Constructors

	// Default constructor
	matrix(int rows, int cols) : rows(rows), cols(cols), data(rows, std::vector<T>(cols, T{})) {}

	// Constructor with initial value
	matrix(int rows, int cols, const T& initialValue) : rows(rows), cols(cols), data(rows, std::vector<T>(cols, initialValue)) {}

	// Constructor with 2D initializer list
	matrix(const std::initializer_list<std::initializer_list<T>>& initList)
		: rows(initList.size()), cols(initList.begin()->size())
	{
		data.reserve(rows);
		for (const auto& rowList : initList)
		{
			data.emplace_back(rowList.begin(), rowList.end());
		}
	}

	// Copy constructor
	matrix(const matrix<T>& other) : rows(other.rows), cols(other.cols), data(other.data) {}

	// Move constructor
	matrix(matrix<T>&& other) noexcept : rows(other.rows), cols(other.cols), data(std::move(other.data)) {}

	#pragma endregion

public:
	#pragma region Operators

	Proxy operator()(int r, int c)
	{
		return Proxy(*this, r, c);
	}

	const Proxy operator()(int r, int c) const
	{
		return Proxy(*const_cast<matrix<T>*>(this), r, c);
	}

	matrix<T>& operator=(const matrix<T>& other)
	{
		if (this != &other)
		{
			rows = other.rows;
			cols = other.cols;
			data = other.data;
		}
		return *this;
	}
	#pragma endregion

public:
	#pragma region User Access Methods

	void SetAllTo(T obj)
	{
		for (int r = 0; r < rows; ++r)
			for (int c = 0; c < cols; ++c)
				data[r][c] = static_cast<T>(obj);
	}

	int Rows() const { return rows; }
	int Cols() const { return cols; }

	void print() const
	{
		for (const auto& row : data)
			fmt::print("{}\n", row);
	}

	template <typename Func>
	void iter(Func func)
	{
		for (int r = 0; r < rows; ++r)
		{
			for (int c = 0; c < cols; ++c)
			{
				func(data[r][c], r, c);
			}
		}
	}

	#pragma endregion
};
