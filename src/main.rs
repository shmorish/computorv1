use regex::Regex;
use std::collections::HashMap;
use std::error::Error;
use std::f64;
use clap::{Arg, Command};

#[derive(Debug, PartialEq)]
struct Term {
    coefficient: f64,
    exponent: i32,
}
/// 多項式をパースする
fn parse_polynomial(input: &str) -> Result<Vec<Term>, String> {
    let re_term = Regex::new(r"(?x)
        (?P<coefficient>[-+]?\d+\.?\d*|[-+]?\.\d+)  # 係数 (空文字列にマッチしないように修正)
        \s*\*?\s*                       # オプションの '*' と空白
        (?:X(?:\^(?P<exponent>\d+))?)?      # 指数 (X^n or X) 部分、省略可能
    ").unwrap();

    let mut terms: HashMap<i32, f64> = HashMap::new();

    for cap in re_term.captures_iter(input) {
        // 係数は必ず存在する (正規表現を変更したため)
        let coefficient_str = cap.name("coefficient").unwrap().as_str(); //ここを修正

        let coefficient = coefficient_str.parse::<f64>()
            .map_err(|_| format!("Invalid coefficient: {}", coefficient_str))?;

         // 指数が省略された場合 (例: 5X)、デフォルトの指数は 1
        let exponent = match cap.name("exponent") {
            Some(exp_match) => exp_match.as_str().parse::<i32>()
                .map_err(|_| format!("Invalid exponent: {}", exp_match.as_str()))?,
            None => {
                // "X" が存在するかどうかで、指数が 0 か 1 かを判定
                if cap.get(0).unwrap().as_str().to_uppercase().contains('X') { 1 } else { 0 }
            }
        };


        // 既存の項と係数を合計する
        *terms.entry(exponent).or_insert(0.0) += coefficient;
    }

    // HashMap を Vec<Term> に変換し、指数でソートする
    let mut sorted_terms: Vec<Term> = terms
        .into_iter()
        .map(|(exponent, coefficient)| Term { coefficient, exponent })
        .collect();
    sorted_terms.sort_by(|a, b| b.exponent.cmp(&a.exponent));

    Ok(sorted_terms)
}

/// 多項式を標準形に変換する
fn reduce_polynomial(left_terms: Vec<Term>, mut right_terms: Vec<Term>) -> Vec<Term> {

    // 右辺の項を左辺に移動し、符号を反転する
    for term in &mut right_terms {
        term.coefficient *= -1.0;
    }

    let mut combined_terms = left_terms;
    combined_terms.extend(right_terms);

    // 指数ごとに係数を合計
    let mut reduced_terms: HashMap<i32, f64> = HashMap::new();
    for term in combined_terms {
        *reduced_terms.entry(term.exponent).or_insert(0.0) += term.coefficient;
    }

    // HashMapをVec<Term>に変換し、指数でソートする
    let mut sorted_reduced_terms: Vec<Term> = reduced_terms.into_iter()
        .map(|(exponent, coefficient)| Term{coefficient, exponent})
        .filter(|term| term.coefficient.abs() > f64::EPSILON) // 係数が0の項を取り除く
        .collect();

    sorted_reduced_terms.sort_by(|a,b| b.exponent.cmp(&a.exponent));

    sorted_reduced_terms
}

// 多項式を表示可能な文字列にする
fn format_polynomial(terms: &[Term]) -> String {

    if terms.is_empty() {
        return "0 = 0".to_string();
    }

    let mut result = String::new();

    for (i, term) in terms.iter().enumerate() {
        let coefficient = term.coefficient;
        let exponent = term.exponent;

        if i > 0 {
            result.push_str(if coefficient >= 0.0 { " + " } else { " - " });
        } else if coefficient < 0.0 {
            result.push('-');
        }


        let abs_coefficient = coefficient.abs();
        if abs_coefficient != 1.0 || exponent == 0 {
             // 小数点以下が0なら表示しない
            result.push_str(&format!("{}", if abs_coefficient.fract() == 0.0 {
                abs_coefficient as f64
            } else {
                abs_coefficient
            }));

        }


        if exponent > 0 {
            result.push('X');
            if exponent > 1 {
                result.push_str(&format!("^{}", exponent));
            }
        }
    }
    result.push_str(" = 0");
    result

}


// 多項式の次数を求める
fn polynomial_degree(terms: &[Term]) -> i32 {
    terms.iter().map(|term| term.exponent).max().unwrap_or(0)
}

// 判別式、解を求める
fn solve_quadratic_equation(terms: &[Term]) -> Result<(), Box<dyn Error>> {
    let degree = polynomial_degree(terms);

    if degree > 2 {
        println!("The polynomial degree is strictly greater than 2, I can't solve.");
        return Ok(());
    }

    // 係数を取得 (存在しない場合は 0)
    let a = terms.iter().find(|t| t.exponent == 2).map_or(0.0, |t| t.coefficient);
    let b = terms.iter().find(|t| t.exponent == 1).map_or(0.0, |t| t.coefficient);
    let c = terms.iter().find(|t| t.exponent == 0).map_or(0.0, |t| t.coefficient);


    if degree == 0 {
        if c == 0.0 {
            println!("Any real number is a solution.");
        }
        else {
            println!("No solution.");
        }
        return Ok(());
    }

    // 1次方程式
    if degree == 1 {
        let solution = -c / b;
        println!("The solution is:");
        println!("{}", solution);
        return Ok(());
    }

    // 2次方程式の解の公式
    let discriminant = b * b - 4.0 * a * c;
    println!("Discriminant: {}", discriminant);

    if discriminant > 0.0 {
        let x1 = (-b + discriminant.sqrt()) / (2.0 * a);
        let x2 = (-b - discriminant.sqrt()) / (2.0 * a);
        println!("Discriminant is strictly positive, the two solutions are:");
        println!("{}", x1);
        println!("{}", x2);
    } else if discriminant == 0.0 {
        let x = -b / (2.0 * a);
        println!("Discriminant is zero, the solution is:");
        println!("{}", x);
    } else {
        let real_part = -b / (2.0 * a);
        let imaginary_part = (-discriminant).sqrt() / (2.0 * a);
        println!("Discriminant is strictly negative, the two complex solutions are:");
        println!("{} + {}i", real_part, imaginary_part);
        println!("{} - {}i", real_part, imaginary_part);
    }

    Ok(())
}


fn main() -> Result<(), Box<dyn Error>> {

    let matches = Command::new("Computor v1")
        .version("0.0.1")
        .about("Solves polynomial equations of degree <= 2.") // About の記述
        .arg(
            Arg::new("polynomial")
                .required(true)
                .help("\"Ao * X^0 + ... + An * X^n = 0\" (Polynomial equation as a string).") // Arg の記述
        )
        .get_matches();

    let input = matches.get_one::<String>("polynomial").expect("required");

    let equation_parts: Vec<&str> = input.split('=').collect();
    if equation_parts.len() != 2 {
        return Err("Invalid equation format. Use '=' to separate sides.".into());
    }

    let left_poly = parse_polynomial(equation_parts[0])?;
    let right_poly = parse_polynomial(equation_parts[1])?;

    let reduced_polynomial = reduce_polynomial(left_poly, right_poly);
    let formatted_polynomial = format_polynomial(&reduced_polynomial);
    let degree = polynomial_degree(&reduced_polynomial);

    println!("Reduced form: {}", formatted_polynomial);
    println!("Polynomial degree: {}", degree);

    solve_quadratic_equation(&reduced_polynomial)?;

    Ok(())
}