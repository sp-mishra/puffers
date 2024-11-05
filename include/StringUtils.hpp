#pragma once

#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <Eigen/Dense>
#include <Eigen/Cholesky>

namespace groklab {
    struct StringUtils {
        // Specialization for Eigen matrices
        template<typename Derived>
        static std::string toString(const Eigen::MatrixBase<Derived>& matrix) {
            std::stringstream ss;
            ss << matrix;
            return ss.str();
        }

        // Generic template for other types
        template<typename T>
        static std::string toString(const T& value) {
            if constexpr (std::is_arithmetic_v<T>) {
                return std::to_string(value);
            } else if constexpr (std::is_same_v<T, std::string>) {
                return value;
            } else if constexpr (is_vector_v<T>) {
                return vectorToString(value);
            } else if constexpr (is_pair_v<T>) {
                return pairToString(value);
            } else if constexpr (is_map_v<T>) {
                return mapToString(value);
            } else if constexpr (is_eigen_matrix_v<T>) {
                return eigenMatrixToString(value);
            } else if constexpr (is_eigen_product_v<T>) {
                return eigenProductToString(value);
            } else if constexpr (is_eigen_array_v<T>) {
                return eigenArrayToString(value);
            } else if constexpr (is_eigen_block_v<T>) {
                return eigenBlockToString(value);
            } else if constexpr (is_eigen_transpose_v<T>) {
                return eigenTransposeToString(value);
            } else if constexpr (is_eigen_diagonal_v<T>) {
                return eigenDiagonalToString(value);
            } else if constexpr (is_eigen_inverse_v<T>) {
                return eigenInverseToString(value);
            } else if constexpr (is_eigen_llt_v<T>) {
                return eigenLLTToString(value);
            } else {
                static_assert(!std::is_same_v<T, T>, "Unsupported type for toString");
            }
            return "";
        }

    private:
        // Helper to check if a type is a vector
        template<typename T>
        struct is_vector : std::false_type {};

        template<typename T, typename A>
        struct is_vector<std::vector<T, A>> : std::true_type {};

        template<typename T>
        static constexpr bool is_vector_v = is_vector<T>::value;

        // Helper to check if a type is a pair
        template<typename T>
        struct is_pair : std::false_type {};

        template<typename T1, typename T2>
        struct is_pair<std::pair<T1, T2>> : std::true_type {};

        template<typename T>
        static constexpr bool is_pair_v = is_pair<T>::value;

        // Helper to check if a type is a map
        template<typename T>
        struct is_map : std::false_type {};

        template<typename K, typename V, typename C, typename A>
        struct is_map<std::map<K, V, C, A>> : std::true_type {};

        template<typename T>
        static constexpr bool is_map_v = is_map<T>::value;

        // Helper to check if a type is an Eigen matrix
        template<typename T>
        struct is_eigen_matrix : std::false_type {};

        template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_eigen_matrix<Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> : std::true_type {};

        template<typename T>
        static constexpr bool is_eigen_matrix_v = is_eigen_matrix<T>::value;

        // Helper to check if a type is an Eigen product
        template<typename T>
        struct is_eigen_product : std::false_type {};

        template<typename Lhs, typename Rhs, int Options>
        struct is_eigen_product<Eigen::Product<Lhs, Rhs, Options>> : std::true_type {};

        template<typename T>
        static constexpr bool is_eigen_product_v = is_eigen_product<T>::value;

        // Helper to check if a type is an Eigen array
        template<typename T>
        struct is_eigen_array : std::false_type {};

        template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
        struct is_eigen_array<Eigen::Array<Scalar, Rows, Cols, Options, MaxRows, MaxCols>> : std::true_type {};

        template<typename T>
        static constexpr bool is_eigen_array_v = is_eigen_array<T>::value;

        // Helper to check if a type is an Eigen block
        template<typename T>
        struct is_eigen_block : std::false_type {};

        template<typename XprType, int BlockRows, int BlockCols, bool InnerPanel>
        struct is_eigen_block<Eigen::Block<XprType, BlockRows, BlockCols, InnerPanel>> : std::true_type {};

        template<typename T>
        static constexpr bool is_eigen_block_v = is_eigen_block<T>::value;

        // Helper to check if a type is an Eigen transpose
        template<typename T>
        struct is_eigen_transpose : std::false_type {};

        template<typename MatrixType>
        struct is_eigen_transpose<Eigen::Transpose<MatrixType>> : std::true_type {};

        template<typename T>
        static constexpr bool is_eigen_transpose_v = is_eigen_transpose<T>::value;

        // Helper to check if a type is an Eigen diagonal
        template<typename T>
        struct is_eigen_diagonal : std::false_type {};

        template<typename MatrixType>
        struct is_eigen_diagonal<Eigen::Diagonal<MatrixType>> : std::true_type {};

        template<typename T>
        static constexpr bool is_eigen_diagonal_v = is_eigen_diagonal<T>::value;

        // Helper to check if a type is an Eigen inverse
        template<typename T>
        struct is_eigen_inverse : std::false_type {};

        template<typename MatrixType>
        struct is_eigen_inverse<Eigen::Inverse<MatrixType>> : std::true_type {};

        template<typename T>
        static constexpr bool is_eigen_inverse_v = is_eigen_inverse<T>::value;

        // Helper to check if a type is an Eigen LLT
        template<typename T>
        struct is_eigen_llt : std::false_type {};

        template<typename MatrixType>
        struct is_eigen_llt<Eigen::LLT<MatrixType>> : std::true_type {};

        template<typename T>
        static constexpr bool is_eigen_llt_v = is_eigen_llt<T>::value;

        // Convert vector to string
        template<typename T>
        static std::string vectorToString(const std::vector<T>& vec) {
            std::stringstream ss;
            ss << "[\n";
            for (size_t i = 0; i < vec.size(); ++i) {
                ss << "  " << toString(vec[i]);
                if (i != vec.size() - 1) {
                    ss << ",\n";
                }
            }
            ss << "\n]";
            return ss.str();
        }

        // Convert pair to string
        template<typename T1, typename T2>
        static std::string pairToString(const std::pair<T1, T2>& p) {
            std::stringstream ss;
            ss << "(\n  " << toString(p.first) << ",\n  " << toString(p.second) << "\n)";
            return ss.str();
        }

        // Convert map to string
        template<typename K, typename V>
        static std::string mapToString(const std::map<K, V>& m) {
            std::stringstream ss;
            ss << "{\n";
            for (auto it = m.begin(); it != m.end(); ++it) {
                ss << "  " << toString(it->first) << ": " << toString(it->second);
                if (std::next(it) != m.end()) {
                    ss << ",\n";
                }
            }
            ss << "\n}";
            return ss.str();
        }

        // Convert Eigen matrix to string
        template<typename T>
        static std::string eigenMatrixToString(const T& matrix) {
            std::stringstream ss;
            ss << matrix;
            return ss.str();
        }

        // Convert Eigen product to string
        template<typename T>
        static std::string eigenProductToString(const T& product) {
            std::stringstream ss;
            ss << product;
            return ss.str();
        }

        // Convert Eigen array to string
        template<typename T>
        static std::string eigenArrayToString(const T& array) {
            std::stringstream ss;
            ss << array;
            return ss.str();
        }

        // Convert Eigen block to string
        template<typename T>
        static std::string eigenBlockToString(const T& block) {
            std::stringstream ss;
            ss << block;
            return ss.str();
        }

        // Convert Eigen transpose to string
        template<typename T>
        static std::string eigenTransposeToString(const T& transpose) {
            std::stringstream ss;
            ss << transpose;
            return ss.str();
        }

        // Convert Eigen diagonal to string
        template<typename T>
        static std::string eigenDiagonalToString(const T& diagonal) {
            std::stringstream ss;
            ss << diagonal;
            return ss.str();
        }

        // Convert Eigen inverse to string
        template<typename T>
        static std::string eigenInverseToString(const T& inverse) {
            std::stringstream ss;
            ss << inverse;
            return ss.str();
        }

        // Convert Eigen LLT to string
        template<typename T>
        static std::string eigenLLTToString(const T& llt) {
            std::stringstream ss;
            ss << llt.matrixL();
            return ss.str();
        }
    };
}
#endif //STRINGUTILS_HPP