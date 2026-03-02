# Лабораторна робота №3 

## Опис завдання

Ця лабораторна робота присвячена візуалізації та аналізу даних датасету Adult (Census Income) з UCI Machine Learning Repository.

## Вимоги до системи
- **Python:** 3.8+
- **Jupyter Notebook**

## Встановлення залежностей
```bash
python -m venv venv
venv\Scripts\activate
```

## Запуск
- jupyter notebook
  
## Датасет

**Назва:** Adult (Census Income) Dataset  
**Джерело:** [UCI ML Repository](https://archive.ics.uci.edu/dataset/2/adult)

**Характеристики датасету:**
- ✅ Dataset Characteristics: Multivariate
- ✅ Attribute Characteristics: Categorical, Integer, Real
- ✅ Number of Attributes: 15
- ✅ Has Missing Values: Yes (~4,262 пропусків)

**Змінні датасету:**
- `age` - вік
- `workclass` - тип роботодавця
- `education` - рівень освіти
- `occupation` - професія
- `hours_per_week` - години роботи на тиждень
- `income` - дохід (≤50K або >50K)
- та інші...

## Виконані завдання

### 1. Data Cleaning
- Виявлено 4,262 пропущених значень у 3 колонках
- Видалено рядки з пропусками
- Фінальний датасет: 30,162 записів

### 2. Графіки залежності
- **Scatter Plot:** Вік vs Години роботи (з розділенням за доходом)
- **Line Plot:** Середні години роботи за віком та статтю

### 3. Гістограма
- Розподіл людей за 5 віковими діапазонами (17-25, 25-35, 35-45, 45-55, 55-90)

### 4. Багатовимірна візуалізація
- **Correlation Heatmap** - кореляційна матриця
- **Box Plot** - розподіл показників за статтю та доходом
- **Violin Plot** - детальний розподіл

### 5. Додаткові графіки
- **Bar Chart** - вплив освіти на дохід
- **Interactive Plotly Scatter** - інтерактивна візуалізація

**Загальна кількість графіків:** 8
