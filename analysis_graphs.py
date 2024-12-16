import pandas as pd
import plotly.express as px
import plotly.graph_objects as go

# Input CSV file
input_file = 'performance_data.csv'

# Read the CSV file into a DataFrame
df = pd.read_csv(input_file)

# Ensure the column names are properly formatted
required_columns = ['MAT_SIZE', 'TILE_SIZE', 'BINARY', 'INSTRUCTIONS', 'CACHE_MISSES', 'ELAPSED_TIME']
if not all(col in df.columns for col in required_columns):
    print("Missing required columns in the CSV file.")
    exit()

# Cache Misses vs. Matrix Size
fig1 = px.scatter(df, x='MAT_SIZE', y='CACHE_MISSES', color='BINARY', 
                  title='Cache Misses vs. Matrix Size', 
                  labels={'MAT_SIZE': 'Matrix Size', 'CACHE_MISSES': 'Cache Misses'})
fig1.update_traces(mode='markers+lines')

# Instructions vs. Tile Size
fig2 = px.scatter(df, x='TILE_SIZE', y='INSTRUCTIONS', color='BINARY', 
                  title='Instructions vs. Tile Size', 
                  labels={'TILE_SIZE': 'Tile Size', 'INSTRUCTIONS': 'Instructions'})
fig2.update_traces(mode='markers+lines')

# Elapsed Time vs. Matrix Size
fig3 = px.scatter(df, x='MAT_SIZE', y='ELAPSED_TIME', color='BINARY', 
                  title='Elapsed Time vs. Matrix Size', 
                  labels={'MAT_SIZE': 'Matrix Size', 'ELAPSED_TIME': 'Elapsed Time (seconds)'})
fig3.update_traces(mode='markers+lines')

# Heatmap of Cache Misses for (Matrix Size, Tile Size)
heatmap_df = df.groupby(['MAT_SIZE', 'TILE_SIZE'])['CACHE_MISSES'].mean().reset_index()
heatmap_pivot = heatmap_df.pivot('MAT_SIZE', 'TILE_SIZE', 'CACHE_MISSES')

fig4 = go.Figure(data=go.Heatmap(
    z=heatmap_pivot.values,
    x=heatmap_pivot.columns,
    y=heatmap_pivot.index,
    colorscale='Viridis'
))
fig4.update_layout(
    title='Heatmap of Cache Misses for (Matrix Size, Tile Size)',
    xaxis_title='Tile Size',
    yaxis_title='Matrix Size'
)

# Save and display plots
fig1.write_html('cache_misses_vs_matrix_size.html')
fig2.write_html('instructions_vs_tile_size.html')
fig3.write_html('elapsed_time_vs_matrix_size.html')
fig4.write_html('heatmap_cache_misses.html')

fig1.show()
fig2.show()
fig3.show()
fig4.show()

print("Graphs have been saved as HTML files and displayed in the browser.")
