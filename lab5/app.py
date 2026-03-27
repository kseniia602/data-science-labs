import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Налаштування сторінки
st.set_page_config(layout="wide", page_title="Аналіз VHI індексів України")

# Функція для завантаження даних
@st.cache_data
def load_data():
    # Шлях до файлу, який ти створила в ЛР2
    file_path = "vhi_all_provinces.csv" 
    try:
        df = pd.read_csv(file_path)
        return df
    except FileNotFoundError:
        st.error(f"Файл {file_path} не знайдено! Переконайся, що він у папці з проектом.")
        return pd.DataFrame()

df = load_data()

# --- ЛОГІКА СКИДАННЯ ФІЛЬТРІВ (Session State) ---
def reset_values():
    st.session_state.index_type = "VHI"
    st.session_state.province = "Черкаська"
    st.session_state.week_range = (1, 52)
    st.session_state.year_range = (1982, 2024)
    st.session_state.sort_asc = False
    st.session_state.sort_desc = False

# Ініціалізація початкових значень
if 'index_type' not in st.session_state:
    reset_values()

# --- САЙДБАР (Панель керування) ---
st.sidebar.header("Налаштування фільтрів")

# 1. Вибір індексу
index_choice = st.sidebar.selectbox(
    "Оберіть індекс:", 
    ["VCI", "TCI", "VHI"], 
    key="index_type"
)

# 2. Вибір області (використовуємо назви з твого списку)
provinces = sorted(df['province_name'].unique()) if not df.empty else []
province_choice = st.sidebar.selectbox(
    "Оберіть область:", 
    options=provinces, 
    key="province"
)

# 3. Слайдер для тижнів
week_range = st.sidebar.slider(
    "Інтервал тижнів:", 1, 52, 
    key="week_range"
)

# 4. Слайдер для років
min_year = int(df['year'].min()) if not df.empty else 1982
max_year = int(df['year'].max()) if not df.empty else 2024
year_range = st.sidebar.slider(
    "Інтервал років:", min_year, max_year, 
    key="year_range"
)

# 5. Сортування (Checkbox)
st.sidebar.subheader("Сортування")
sort_asc = st.sidebar.checkbox("За зростанням", key="sort_asc")
sort_desc = st.sidebar.checkbox("За спаданням", key="sort_desc")

# Перевірка на вибір обох чекбоксів
if sort_asc and sort_desc:
    st.sidebar.warning("⚠️ Вибрано обидва типи сортування. Буде використано сортування 'За зростанням'.")

# 6. Кнопка Reset
st.sidebar.button("Скинути фільтри", on_click=reset_values)

# --- ФІЛЬТРАЦІЯ ТА ОБРОБКА ДАНИХ ---
if not df.empty:
    # Основна фільтрація
    filtered_df = df[
        (df['province_name'] == province_choice) &
        (df['year'] >= year_range[0]) & (df['year'] <= year_range[1]) &
        (df['week'] >= week_range[0]) & (df['week'] <= week_range[1])
    ]

    # Логіка сортування
    if sort_asc:
        filtered_df = filtered_df.sort_values(by=index_choice, ascending=True)
    elif sort_desc:
        filtered_df = filtered_df.sort_values(by=index_choice, ascending=False)

    # --- ОСНОВНИЙ КОНТЕНТ ---
    st.title(f"Дослідження індексу {index_choice} для України")
    
    # Створюємо вкладки
    tab1, tab2, tab3 = st.tabs(["📋 Таблиця даних", "📈 Графік області", "📊 Порівняння областей"])

    with tab1:
        st.subheader(f"Відфільтровані дані для області: {province_choice}")
        st.dataframe(filtered_df, use_container_width=True)

    with tab2:
        st.subheader(f"Динаміка {index_choice} у часі")
        if not filtered_df.empty:
            # Для графіка часового ряду краще сортувати за часом назад для візуалізації
            plot_df = filtered_df.sort_values(['year', 'week'])
            
            fig, ax = plt.subplots(figsize=(10, 5))
            # Створюємо мітку часу для осі X
            plot_df['date_label'] = plot_df['year'].astype(str) + "-w" + plot_df['week'].astype(str)
            
            ax.plot(plot_df['date_label'], plot_df[index_choice], color='green', linewidth=1.5)
            
            # Налаштування відображення підписів (щоб не було занадто густо)
            n = max(1, len(plot_df) // 15)
            plt.xticks(plot_df['date_label'][::n], rotation=45)
            
            ax.set_xlabel("Рік та Тиждень")
            ax.set_ylabel(index_choice)
            ax.grid(True, alpha=0.3)
            st.pyplot(fig)
        else:
            st.info("Немає даних для побудови графіка за такими фільтрами.")

    with tab3:
        st.subheader(f"Порівняння середнього {index_choice} по областях")
        
        # Дані для порівняння (всі області за той самий часовий проміжок)
        comp_df = df[
            (df['year'] >= year_range[0]) & (df['year'] <= year_range[1]) &
            (df['week'] >= week_range[0]) & (df['week'] <= week_range[1])
        ]
        
        if not comp_df.empty:
            # Групуємо за назвою області
            avg_all = comp_df.groupby('province_name')[index_choice].mean().sort_values().reset_index()
            
            fig2, ax2 = plt.subplots(figsize=(10, 6))
            # Робимо колір обраної області іншим
            colors = ['orange' if name == province_choice else 'skyblue' for name in avg_all['province_name']]
            
            sns.barplot(data=avg_all, x=index_choice, y='province_name', palette=colors, ax=ax2, hue='province_name', legend=False)
            
            ax2.set_title(f"Середнє значення {index_choice} за обраний період")
            st.pyplot(fig2)
        else:
            st.info("Немає даних для порівняння.")

else:
    st.error("Помилка завантаження даних. Перевірте CSV файл.")